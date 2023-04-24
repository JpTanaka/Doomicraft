#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.hpp"
#include "audio_controller.hpp"


listeners lists;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void init_sound_effects(){
	std::thread gunshot(init_sfx, "../assets/gunshot_2.wav", 100, &(lists.shoot), false);
	gunshot.detach();

    std::thread hit(init_sfx, "../assets/bullet_hit.wav", 100, &(lists.hit), false);
	hit.detach();

	std::thread kill(init_sfx, "../assets/creeper_death.wav", 1000, &(lists.kill), false);
	kill.detach();

	std::thread death(init_sfx, "../assets/death_bell.wav", 4500, &(lists.death), false);
	death.detach();

	std::thread place_block(init_sfx, "../assets/place_block.wav", 140, &(lists.place_block), false);
	place_block.detach();

	std::thread background(init_sfx, "../assets/background_song.wav", 138000, &(lists.background), true);
	background.detach();

	std::thread background_creative(init_sfx, "../assets/background_song_calm.mp3", 175000, &(lists.background_creative), true);
	background_creative.detach();
}

void init_sfx(const char* wav_path, int duration_ms, bool* listener, bool on_repeat) {
	for(;;){
        std::cout << std::endl; // dont know why but this is needed to make audio run on windows ...
		if (!*listener) continue;
		if (!on_repeat) *listener = false;

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

