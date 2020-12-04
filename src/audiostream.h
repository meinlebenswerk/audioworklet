#pragma once


// #include "napi-thread-safe-callback.hpp"
#include <napi.h>
#include <iostream>
#include "cubeb/cubeb.h"
#include "class_registry.h"
#include "audio_ring_buffer.h"

#include <thread>

int bytesPerFormat(cubeb_sample_format format);

struct audioBufferWrapper {
	const void * buffer;
	long nframes;
	uint8_t bytesPerFrame;
	uint8_t nChannels;
};

class AudioStream : public Napi::ObjectWrap<AudioStream>
{
public:
	static void Init(Napi::Env& env, Napi::Object exports, ClassRegistry *registry);

	AudioStream(const Napi::CallbackInfo &info);
	~AudioStream();

	Napi::Value isStarted(const Napi::CallbackInfo& info);
	void start(const Napi::CallbackInfo &info);
	void stop(const Napi::CallbackInfo &info);
	void setVolume(const Napi::CallbackInfo &info);

	Napi::Value getLatency(const Napi::CallbackInfo& info);
	Napi::Value getPosition(const Napi::CallbackInfo& info);
	Napi::Value pushAudioChunk(const Napi::CallbackInfo &info);
	Napi::Value readAudioChunk(const Napi::CallbackInfo &info);
	Napi::Value getBufferSize(const Napi::CallbackInfo &info);
	Napi::Value getBufferCapacity(const Napi::CallbackInfo &info);

	Napi::Value getFormat(const Napi::CallbackInfo &info);
	Napi::Value getChannels(const Napi::CallbackInfo &info);
	Napi::Value getRate(const Napi::CallbackInfo &info);
	void registerAudioCallback(const Napi::CallbackInfo &info);

	friend long data_callback(cubeb_stream *stream, void *user_ptr, void const *input_buffer, void *output_buffer, long nframes);
	friend void state_callback(cubeb_stream * stm, void * user, cubeb_state state);
	friend void state_callback_js(const Napi::CallbackInfo& info);

private:
	cubeb_stream *_stream;
	cubeb *_cubebContext;
	cubeb_stream_params _params;

	uint32_t _configuredLatencyFrames;
	uint32_t _bufferCapacityFrames;

	bool _isStarted;
	uint64_t _lastChunkTimestamp;
	Napi::ThreadSafeFunction _stateCallbackfn;
	Napi::Reference<Napi::Value> _ownRef;
	Napi::Reference<Napi::Value> _parentRef;
	bool _isInput;
	bool _logProcessTime;
	std::unique_ptr<AudioRingBuffer> _audioBuffer;
	uint64_t _timestamp;

	uint8_t nChannels;
	uint8_t bytesPerSample;

	bool hasCallback;
	Napi::ThreadSafeFunction threadsafeCallback;
	// std::thread nativeThread;
};

