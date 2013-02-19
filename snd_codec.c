/*
 * Audio Codecs: Adapted from ioquake3 with changes.
 * For now, only handles streaming music, not sound effects.
 *
 * Copyright (C) 1999-2005 Id Software, Inc.
 * Copyright (C) 2005 Stuart Dalton <badcdev@gmail.com>
 * Copyright (C) 2010-2011 O.Sezer <sezero@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "quakedef.h"
#include "snd_codec.h"
#include "snd_codeci.h"

/* headers for individual codecs */
#include "snd_wave.h"
#include "snd_mp3.h"
#include "snd_vorbis.h"


static snd_codec_t *codecs;

/*
=================
S_CodecRegister
=================
*/
static void S_CodecRegister(snd_codec_t *codec)
{
	codec->next = codecs;
	codecs = codec;
}

/*
=================
S_CodecInit
=================
*/
void S_CodecInit (void)
{
	snd_codec_t *codec;
	codecs = NULL;

	/* Register in the inverse order
	 * of codec choice preference: */
#ifdef USE_CODEC_WAVE
	S_CodecRegister(&wav_codec);
#endif
#ifdef USE_CODEC_MP3
	S_CodecRegister(&mp3_codec);
#endif
#ifdef USE_CODEC_VORBIS
	S_CodecRegister(&ogg_codec);
#endif

	codec = codecs;
	while (codec)
	{
		codec->initialize();
		codec = codec->next;
	}
}

/*
=================
S_CodecShutdown
=================
*/
void S_CodecShutdown (void)
{
	snd_codec_t *codec = codecs;
	while (codec)
	{
		codec->shutdown();
		codec = codec->next;
	}
	codecs = NULL;
}

/*
=================
S_CodecOpenStream
=================
*/
snd_stream_t *S_CodecOpenStreamType (const char *filename, unsigned int type)
{
	snd_codec_t *codec;
	snd_stream_t *stream;

	if (type == CODECTYPE_NONE)
	{
		Con_Printf("Bad type for %s\n", filename);
		return NULL;
	}

	codec = codecs;
	while (codec)
	{
		if (type == codec->type)
			break;
		codec = codec->next;
	}
	if (!codec)
	{
		Con_Printf("Unknown type for %s\n", filename);
		return NULL;
	}
	stream = codec->codec_open(filename);
	if (stream)
		stream->status = STREAM_PLAY;
	return stream;
}

snd_stream_t *S_CodecOpenStreamExt (const char *filename)
{
	snd_codec_t *codec;
	snd_stream_t *stream;
	const char *ext;

	ext = COM_FileExtension(filename);
	if (! *ext)
	{
		Con_Printf("No extension for %s\n", filename);
		return NULL;
	}

	codec = codecs;
	while (codec)
	{
		if (!Q_strcasecmp(ext, codec->ext))
			break;
		codec = codec->next;
	}
	if (!codec)
	{
		Con_Printf("Unknown extension for %s\n", filename);
		return NULL;
	}
	stream = codec->codec_open(filename);
	if (stream)
		stream->status = STREAM_PLAY;
	return stream;
}

snd_stream_t *S_CodecOpenStreamAny (const char *filename)
{
	snd_codec_t *codec;
	snd_stream_t *stream;
	const char *ext;

	ext = COM_FileExtension(filename);
	if (! *ext)	/* try all available */
	{
		char tmp[MAX_QPATH];

		codec = codecs;
		while (codec)
		{
			Q_snprintfz(tmp, sizeof(tmp), "%s.%s", filename, codec->ext);
			stream = codec->codec_open(tmp);
			if (stream)
			{
				stream->status = STREAM_PLAY;
				return stream;
			}
			codec = codec->next;
		}

		return NULL;
	}
	else	/* use the name as is */
	{
		codec = codecs;
		while (codec)
		{
			if (!Q_strcasecmp(ext, codec->ext))
				break;
			codec = codec->next;
		}
		if (!codec)
		{
			Con_Printf("Unknown extension for %s\n", filename);
			return NULL;
		}
		stream = codec->codec_open(filename);
		if (stream)
			stream->status = STREAM_PLAY;
		return stream;
	}
}

void S_CodecCloseStream (snd_stream_t *stream)
{
	stream->status = STREAM_NONE;
	stream->codec->codec_close(stream);
}

int S_CodecRewindStream (snd_stream_t *stream)
{
	return stream->codec->codec_rewind(stream);
}

int S_CodecReadStream (snd_stream_t *stream, int bytes, void *buffer)
{
	return stream->codec->codec_read(stream, bytes, buffer);
}

/* Util functions (used by codecs) */

snd_stream_t *S_CodecUtilOpen(const char *filename, snd_codec_t *codec)
{
	snd_stream_t *stream;
	FILE *handle;
	qboolean pak;
	long length;

	/* Try to open the file */
	length = (long) COM_FOpenFile(filename, &handle, NULL);
	pak = file_from_pak;
	if (length == -1)
	{
		Con_DPrintf("Couldn't open %s\n", filename);
		return NULL;
	}

	/* Allocate a stream, malloc zeroes its content */
	stream = (snd_stream_t *) Q_malloc(sizeof(snd_stream_t));
	stream->codec = codec;
	stream->fh.file = handle;
	stream->fh.start = ftell(handle);
	stream->fh.pos = 0;
	stream->fh.length = length;
	stream->fh.pak = stream->pak = pak;
	return stream;
}

void S_CodecUtilClose(snd_stream_t **stream)
{
	fclose((*stream)->fh.file);
	Q_free(*stream);
	*stream = NULL;
}

int S_CodecIsAvailable (unsigned int type)
{
	snd_codec_t *codec = codecs;
	while (codec)
	{
		if (type == codec->type)
			return codec->initialized;
		codec = codec->next;
	}
	return -1;
}

