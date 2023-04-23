#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "audio_controller.hpp"


listeners lists = {false, false, false};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void init_sound_effects(){
	std::thread gunshot(init_sfx, "../assets/gunshot_2.wav", 100, &(lists.shoot));
	gunshot.detach();

	std::thread hit(init_sfx, "../assets/hit_2.wav", 40, &(lists.hit));
	hit.detach();

	std::thread kill(init_sfx, "../assets/death.wav", 400, &(lists.kill));
	kill.detach();
}

void init_sfx(const char* wav_path, int duration_ms, bool* listener) {
	for(;;){

		if (!*listener) continue;
        *listener = false;
 	   	ma_result result;
 	   	ma_decoder decoder;
 	   	result = ma_decoder_init_file(wav_path, NULL, &decoder);
 	   	if (result != MA_SUCCESS) {
 	   	    std::cerr << "Failed to initialize decoder" << std::endl;
 	   	    return;
 	   	}
 	   	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
 	   	deviceConfig.playback.format = decoder.outputFormat;
 	   	deviceConfig.playback.channels = decoder.outputChannels;
 	   	deviceConfig.sampleRate = decoder.outputSampleRate;
 	   	deviceConfig.dataCallback = data_callback;
 	   	deviceConfig.pUserData = &decoder;
 	   	ma_device device;
 	   	result = ma_device_init(NULL, &deviceConfig, &device);
 	   	if (result != MA_SUCCESS) {
 	   	    std::cerr << "Failed to initialize playback device" << std::endl;
 	   	    ma_decoder_uninit(&decoder);
 	   	    return;
 	   	}
 	   	result = ma_device_start(&device);
 	   	if (result != MA_SUCCESS) {
 	   	    std::cerr << "Failed to start playback device" << std::endl;
 	   	    ma_device_uninit(&device);
 	   	    ma_decoder_uninit(&decoder);
 	   	    return;
 	   	}
 	   	// Sleep for the duration of the sound
 	   	std::chrono::milliseconds duration(duration_ms);
 	   	std::this_thread::sleep_for(duration);
 	   	ma_device_uninit(&device);
 	   	ma_decoder_uninit(&decoder);
	}
}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

