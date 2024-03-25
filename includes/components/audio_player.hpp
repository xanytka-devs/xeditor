#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <xengine/rendering/transform.hpp>
#include <../packages/xengine.audio/includes/audio.hpp>

using namespace XEngine;
class AudioPlayer : public Component {
public:
	AudioPlayer() { }
	AudioPlayer(Transform t_transform, const char* t_audio_file = "", bool t_play_on_awake = true)
		: Component(t_transform) {
		m_audio = Audio(t_audio_file, false, master_layer, t_play_on_awake);
	}

	void play() {
		m_audio.play();
	}

	void stop() {
		m_audio.stop();
	}

	void on_destroy() {
		m_audio.remove();
		Component::on_destroy();
	}

	void set_source(const char* t_src) {
		m_audio.set_source(t_src);
	}
	std::string get_source() {
		return m_audio.get_source();
	}
private:
	Audio m_audio;
};

#endif // AUDIO_PLAYER_H