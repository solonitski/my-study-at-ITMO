#include "return_codes.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>

#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>

int print_error(const char *error_text, int error_code)
{
	fprintf(stderr, "%s", error_text);
	return error_code;
}

int check_audio_format(AVFormatContext **format_context, const char *filename, int *audio_stream_index)
{
	// File opening check.
	if (avformat_open_input(format_context, filename, NULL, NULL) != 0)
	{
		return print_error("Error. Failed to open audio file.\n", ERROR_DATA_INVALID);
	}

	// Find stream information.
	if (avformat_find_stream_info(*format_context, NULL) < 0)
	{
		return print_error("Error. Failed to find stream info.\n", ERROR_DATA_INVALID);
	}

	// Find correct stream
	*audio_stream_index = av_find_best_stream(*format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

	// Checking for the presence of an audio stream
	if (*audio_stream_index == -1)
	{
		return print_error("Error. No audio stream found in the file.\n", ERROR_DATA_INVALID);
	}

	// Check codec format
	switch ((*format_context)->streams[*audio_stream_index]->codecpar->codec_id)
	{
	case AV_CODEC_ID_FLAC:
	case AV_CODEC_ID_MP2:
	case AV_CODEC_ID_MP3:
	case AV_CODEC_ID_OPUS:
	case AV_CODEC_ID_AAC:
		break;
	default:
		return print_error("Unsupported audio format.\n", ERROR_DATA_INVALID);
	}

	return SUCCESS;
}

int configure_codec_context(AVFormatContext *format_context, int audio_stream_index, AVCodecContext **codec_ctx)
{
	const AVCodec *codec = avcodec_find_decoder(format_context->streams[audio_stream_index]->codecpar->codec_id);
	if (!codec)
	{
		return print_error("Error. Codec not found.\n", ERROR_DATA_INVALID);
	}

	*codec_ctx = avcodec_alloc_context3(codec);
	if (!(*codec_ctx))
	{
		return print_error("Error. Failed to allocate codec context.\n", ERROR_NOTENOUGH_MEMORY);
	}

	int ret = avcodec_parameters_to_context(*codec_ctx, format_context->streams[audio_stream_index]->codecpar);
	if (ret < 0)
	{
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to copy codec parameters to codec context.\n", ERROR_DATA_INVALID);
	}

	ret = avcodec_open2(*codec_ctx, codec, NULL);
	if (ret < 0)
	{
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to open codec.\n", ERROR_DATA_INVALID);
	}

	return SUCCESS;
}

int extract_audio_data(AVFormatContext *format_context, AVCodecContext **codec_ctx, double **audio_data, int *data_size, int audio_stream_index)
{
	AVPacket *packet = av_packet_alloc();
	if (!packet)
	{
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to allocate memory for packet.\n", ERROR_NOTENOUGH_MEMORY);
	}

	AVFrame *frame = av_frame_alloc();
	if (!frame)
	{
		av_packet_free(&packet);
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to allocate memory for frame.\n", ERROR_NOTENOUGH_MEMORY);
	}

	int nb_samples_total = 0;

	while (av_read_frame(format_context, packet) >= 0)
	{
		if (packet->stream_index == audio_stream_index)
		{
			if (avcodec_send_packet(*codec_ctx, packet) < 0)
			{
				break;
			}

			while (avcodec_receive_frame(*codec_ctx, frame) >= 0)
			{
				int nb_samples = frame->nb_samples;
				*audio_data = (double *)realloc(*audio_data, (nb_samples_total + nb_samples) * sizeof(double));
				if (!(*audio_data))
				{
					return print_error("Error. Failed to reallocate memory for audio data.\n", ERROR_NOTENOUGH_MEMORY);
				}

				for (int i = 0; i < nb_samples; i++)
				{
					(*audio_data)[nb_samples_total + i] = frame->data[0][i];
				}
				nb_samples_total += nb_samples;
			}
		}
		av_packet_unref(packet);
	}

	av_packet_free(&packet);
	av_frame_free(&frame);
	avcodec_free_context(codec_ctx);

	if (nb_samples_total > 0)
	{
		*data_size = nb_samples_total;
		return SUCCESS;
	}
	else
	{
		return print_error("Error. No audio data extracted.\n", ERROR_DATA_INVALID);
	}
}

int extract_audio_data_two_channels(
	AVFormatContext *format_context,
	AVCodecContext **codec_ctx,
	double **audio_data_1,
	double **audio_data_2,
	int *size_1,
	int *size_2,
	int audio_stream_index)
{
	AVPacket *packet = av_packet_alloc();
	if (!packet)
	{
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to allocate memory for packet.\n", ERROR_NOTENOUGH_MEMORY);
	}

	AVFrame *frame = av_frame_alloc();
	if (!frame)
	{
		av_packet_free(&packet);
		avcodec_free_context(codec_ctx);
		return print_error("Error. Failed to allocate memory for frame.\n", ERROR_NOTENOUGH_MEMORY);
	}

	int nb_samples_total_channel1 = 0;
	int nb_samples_total_channel2 = 0;

	while (av_read_frame(format_context, packet) >= 0)
	{
		if (packet->stream_index == audio_stream_index)
		{
			if (avcodec_send_packet(*codec_ctx, packet) < 0)
			{
				break;
			}

			while (avcodec_receive_frame(*codec_ctx, frame) >= 0)
			{
				if (frame->ch_layout.nb_channels != 2)
				{
					av_packet_unref(packet);
					av_frame_free(&frame);
					avcodec_free_context(codec_ctx);
					return print_error("Error. The audio file was expected to have 2 channels.\n", ERROR_FORMAT_INVALID);
				}

				int nb_samples = frame->nb_samples;

				*audio_data_1 = (double *)realloc(*audio_data_1, (nb_samples_total_channel1 + nb_samples) * sizeof(double));
				*audio_data_2 = (double *)realloc(*audio_data_2, (nb_samples_total_channel2 + nb_samples) * sizeof(double));
				if (!(*audio_data_1) || !(*audio_data_2))
				{
					return print_error("Error. Failed to reallocate memory for audio data.\n", ERROR_NOTENOUGH_MEMORY);
				}

				for (int i = 0; i < nb_samples; i++)
				{
					(*audio_data_1)[nb_samples_total_channel1 + i] = frame->data[0][i];
					(*audio_data_2)[nb_samples_total_channel2 + i] = frame->data[1][i];
				}

				nb_samples_total_channel1 += nb_samples;
				nb_samples_total_channel2 += nb_samples;
			}
		}
		av_packet_unref(packet);
	}

	av_packet_free(&packet);
	av_frame_free(&frame);
	avcodec_free_context(codec_ctx);

	if (nb_samples_total_channel1 > 0 && nb_samples_total_channel2 > 0)
	{
		*size_1 = nb_samples_total_channel1;
		*size_2 = nb_samples_total_channel2;
		return SUCCESS;
	}
	else
	{
		return print_error("Error. No audio data extracted.\n", ERROR_DATA_INVALID);
	}
}

void cleanup_fftw(fftw_plan plan_forward1, fftw_plan plan_forward2, fftw_plan plan_backward, fftw_complex *data1_fft, fftw_complex *data2_fft, fftw_complex *cross_corr_fft, double *cross_correlation)
{
	fftw_destroy_plan(plan_forward1);
	fftw_destroy_plan(plan_forward2);
	fftw_destroy_plan(plan_backward);
	fftw_free(data1_fft);
	fftw_free(data2_fft);
	fftw_free(cross_corr_fft);
	free(cross_correlation);
}

int perform_cross_correlation(double *audio_data1, double *audio_data2, int size, int sample_rate, int *delta_samples, int *delta_ms)
{
	fftw_complex *data1_fft, *data2_fft, *cross_corr_fft;
	fftw_plan plan_forward1, plan_forward2, plan_backward;

	// Memory allocation for storing the results of Fourier transformation
	data1_fft = fftw_malloc(sizeof(fftw_complex) * size);
	data2_fft = fftw_malloc(sizeof(fftw_complex) * size);
	cross_corr_fft = fftw_malloc(sizeof(fftw_complex) * size);

	if (!data1_fft || !data2_fft || !cross_corr_fft)
	{
		cleanup_fftw(NULL, NULL, NULL, data1_fft, data2_fft, cross_corr_fft, NULL);
		return print_error("Error. Failed to allocate memory for FFT.\n", ERROR_NOTENOUGH_MEMORY);
	}

	// Creation of Fourier transformation plans
	plan_forward1 = fftw_plan_dft_r2c_1d(size, audio_data1, data1_fft, FFTW_ESTIMATE);
	plan_forward2 = fftw_plan_dft_r2c_1d(size, audio_data2, data2_fft, FFTW_ESTIMATE);

	if (!plan_forward1 || !plan_forward2)
	{
		cleanup_fftw(plan_forward1, plan_forward2, NULL, data1_fft, data2_fft, cross_corr_fft, NULL);
		return print_error("Error. Failed to create FFT plans.\n", ERROR_DATA_INVALID);
	}

	// Execution of Fourier transformations
	fftw_execute(plan_forward1);
	fftw_execute(plan_forward2);

	// Computing cross-correlation in the frequency domain
	for (int i = 0; i < size; i++)
	{
		cross_corr_fft[i][0] = (data1_fft[i][0] * data2_fft[i][0]) + (data1_fft[i][1] * data2_fft[i][1]);
		cross_corr_fft[i][1] = (data1_fft[i][1] * data2_fft[i][0]) - (data1_fft[i][0] * data2_fft[i][1]);
	}

	double *cross_correlation = (double *)malloc(size * sizeof(double));
	if (!cross_correlation)
	{
		cleanup_fftw(plan_forward1, plan_forward2, NULL, data1_fft, data2_fft, cross_corr_fft, cross_correlation);
		return print_error("Error. Failed to allocate memory for cross correlation.\n", ERROR_NOTENOUGH_MEMORY);
	}

	plan_backward = fftw_plan_dft_c2r_1d(size, cross_corr_fft, cross_correlation, FFTW_ESTIMATE);

	if (!plan_backward)
	{
		cleanup_fftw(plan_forward1, plan_forward2, NULL, data1_fft, data2_fft, cross_corr_fft, cross_correlation);
		return print_error("Error. Failed to create backward FFT plan.\n", ERROR_DATA_INVALID);
	}

	// Inverse Fourier Transform
	fftw_execute(plan_backward);

	double max_corr = 0;
	int max_index = 0;
	for (int i = 0; i < size; ++i)
	{
		if (cross_correlation[i] > max_corr)
		{
			max_corr = cross_correlation[i];
			max_index = i;
		}
	}

	*delta_samples = max_index > size / 2 ? max_index - size : max_index;
	*delta_ms = (double)*delta_samples / sample_rate * 1000;

	cleanup_fftw(plan_forward1, plan_forward2, plan_backward, data1_fft, data2_fft, cross_corr_fft, cross_correlation);
	return SUCCESS;
}

int find_audio_delay(double *audio_data1, double *audio_data2, int size, int sample_rate, int *delta_samples, int *delta_ms)
{
	int result = perform_cross_correlation(audio_data1, audio_data2, size, sample_rate, delta_samples, delta_ms);
	if (result != SUCCESS)
	{
		return result;
	}

	printf("delta: %i samples\n", *delta_samples);
	printf("sample rate: %i Hz\n", sample_rate);
	printf("delta time: %i ms\n", *delta_ms);

	return SUCCESS;
}

int extend_and_fill_with_zeros(double **array, int old_size, int new_size)
{
	double *new_array = (double *)malloc(new_size * sizeof(double));
	if (!new_array)
	{
		return print_error("Error. Failed to allocate memory for FFT.\n", ERROR_NOTENOUGH_MEMORY);
	}

	for (int i = 0; i < old_size; ++i)
	{
		new_array[i] = (*array)[i];
	}

	for (int i = old_size; i < new_size; ++i)
	{
		new_array[i] = 0;
	}

	free(*array);
	*array = new_array;

	return SUCCESS;
}

void cleanup_resources(AVFormatContext *format_context_1, AVFormatContext *format_context_2, double *audio_data1, double *audio_data2)
{
	avformat_close_input(&format_context_1);
	avformat_close_input(&format_context_2);
	free(audio_data1);
	free(audio_data2);
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		return print_error("Error. Invalid count of arguments\n", ERROR_ARGUMENTS_INVALID);
	}

	av_log_set_level(AV_LOG_QUIET);
	int err;

	if (argc == 2)
	{
		int audio_stream_index = -1;

		AVFormatContext *format_context = NULL;
		err = check_audio_format(&format_context, argv[1], &audio_stream_index);
		if (err)
		{
			cleanup_resources(format_context, NULL, NULL, NULL);
			return err;
		}

		AVCodecContext *codec_ctx = NULL;
		err = configure_codec_context(format_context, audio_stream_index, &codec_ctx);
		if (err)
		{
			cleanup_resources(format_context, NULL, NULL, NULL);
			return err;
		}

		double *audio_data1 = NULL, *audio_data2 = NULL;
		int size1 = 0, size2 = 0;
		err = extract_audio_data_two_channels(format_context, &codec_ctx, &audio_data1, &audio_data2, &size1, &size2, audio_stream_index);
		if (err != SUCCESS)
		{
			cleanup_resources(format_context, NULL, audio_data1, audio_data2);
			return err;
		}

		int new_size = size1 + size2 - 1;

		err = extend_and_fill_with_zeros(&audio_data1, size1, new_size);
		if (err != SUCCESS)
		{
			cleanup_resources(format_context, NULL, audio_data1, audio_data2);
			return err;
		}

		err = extend_and_fill_with_zeros(&audio_data2, size2, new_size);
		if (err != SUCCESS)
		{
			cleanup_resources(format_context, NULL, audio_data1, audio_data2);
			return err;
		}

		int sample_rate = format_context->streams[audio_stream_index]->codecpar->sample_rate;
		int delta_samples = 0, delta_ms = 0;

		err = find_audio_delay(audio_data1, audio_data2, new_size, sample_rate, &delta_samples, &delta_ms);
		if (err != SUCCESS)
		{
			cleanup_resources(format_context, NULL, audio_data1, audio_data2);
			return err;
		}

		cleanup_resources(format_context, NULL, audio_data1, audio_data2);
	}
	else
	{
		int first_audio_stream_index = -1;
		int second_audio_stream_index = -1;

		AVFormatContext *first_format_context = NULL;
		err = check_audio_format(&first_format_context, argv[1], &first_audio_stream_index);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, NULL, NULL, NULL);
			return err;
		}

		AVCodecContext *first_codec_ctx = NULL;
		err = configure_codec_context(first_format_context, first_audio_stream_index, &first_codec_ctx);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, NULL, NULL, NULL);
			return err;
		}

		AVFormatContext *second_format_context = NULL;
		err = check_audio_format(&second_format_context, argv[2], &second_audio_stream_index);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, NULL, NULL);
			return err;
		}

		AVCodecContext *second_codec_ctx = NULL;
		err = configure_codec_context(second_format_context, second_audio_stream_index, &second_codec_ctx);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, NULL, NULL);
			return err;
		}

		int sample_rate1 = first_format_context->streams[first_audio_stream_index]->codecpar->sample_rate;
		int sample_rate2 = second_format_context->streams[second_audio_stream_index]->codecpar->sample_rate;

		if (sample_rate1 != sample_rate2)
		{
			cleanup_resources(first_format_context, second_format_context, NULL, NULL);
			return print_error("The program currently lacks the capability for resampling.\n", ERROR_UNSUPPORTED);
		}

		double *audio_data1 = NULL;
		int size1 = 0;
		err = extract_audio_data(first_format_context, &first_codec_ctx, &audio_data1, &size1, first_audio_stream_index);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, audio_data1, NULL);
			return err;
		}

		double *audio_data2 = NULL;
		int size2 = 0;
		err = extract_audio_data(second_format_context, &second_codec_ctx, &audio_data2, &size2, second_audio_stream_index);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, audio_data1, audio_data2);
			return err;
		}

		int new_size = size1 + size2 - 1;
		err = extend_and_fill_with_zeros(&audio_data1, size1, new_size);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, audio_data1, audio_data2);
			return err;
		}

		err = extend_and_fill_with_zeros(&audio_data2, size2, new_size);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, audio_data1, audio_data2);
			return err;
		}

		int sample_rate = first_format_context->streams[first_audio_stream_index]->codecpar->sample_rate;
		int delta_samples = 0, delta_ms = 0;

		err = find_audio_delay(audio_data1, audio_data2, new_size, sample_rate, &delta_samples, &delta_ms);
		if (err != SUCCESS)
		{
			cleanup_resources(first_format_context, second_format_context, audio_data1, audio_data2);
			return err;
		}

		cleanup_resources(first_format_context, second_format_context, audio_data1, audio_data2);
	}

	return SUCCESS;
}
