#include <libavutil/log.h>

#define LOGD(format, ...) av_log(NULL, AV_LOG_DEBUG, format, ##__VA_ARGS__);

int ff_dump_stream_info(char * url)
{
    AVFormatContext *ic = avformat_alloc_context();

    if (avformat_open_input(&ic, url, NULL, NULL) < 0) {
        LOGD("could not open source %s", url);
        return -1;
    }

    if (avformat_find_stream_info(ic, NULL) < 0) {
        LOGD("could not find stream information");
        return -1;
    }

    LOGD("---------- dumping stream info ----------");

    LOGD("input format: %s", ic->iformat->name);
    LOGD("nb_streams: %d", ic->nb_streams);

    int64_t start_time = ic->start_time / AV_TIME_BASE;
    LOGD("start_time: %lld", start_time);
    
    int64_t duration = ic->duration / AV_TIME_BASE;
    LOGD("duration: %lld s", duration);

    int video_stream_idx = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream_idx >= 0) {
        AVStream *video_stream = ic->streams[video_stream_idx];
        LOGD("video nb_frames: %lld", video_stream->nb_frames);
        LOGD("video codec_id: %d", video_stream->codec->codec_id);
        LOGD("video codec_name: %s", avcodec_get_name(video_stream->codec->codec_id));
        LOGD("video width x height: %d x %d", video_stream->codec->width, video_stream->codec->height);
        LOGD("video pix_fmt: %d", video_stream->codec->pix_fmt);
        LOGD("video bitrate %lld kb/s", (int64_t) video_stream->codec->bit_rate / 1000);
        LOGD("video avg_frame_rate: %d fps", video_stream->avg_frame_rate.num/video_stream->avg_frame_rate.den);
    }
    
    int audio_stream_idx = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream_idx >= 0) {
        AVStream *audio_stream = ic->streams[audio_stream_idx];
        LOGD("audio codec_id: %d", audio_stream->codec->codec_id);
        LOGD("audio codec_name: %s", avcodec_get_name(audio_stream->codec->codec_id));
        LOGD("audio sample_rate: %d", audio_stream->codec->sample_rate);
        LOGD("audio channels: %d", audio_stream->codec->channels);
        LOGD("audio sample_fmt: %d", audio_stream->codec->sample_fmt);
        LOGD("audio frame_size: %d", audio_stream->codec->frame_size);
        LOGD("audio nb_frames: %lld", audio_stream->nb_frames);
        LOGD("audio bitrate %lld kb/s", (int64_t) audio_stream->codec->bit_rate / 1000);
    }

    LOGD("---------- dumping stream info ----------");
    
    avformat_close_input(&ic);

}

