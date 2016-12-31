#include "soundController.hpp"
#include "Engine.hpp"

SoundController::SoundController() { sf::Listener::setGlobalVolume(75.f); }

void SoundController::pause(int options) {
    std::lock_guard<std::mutex> lk(soundsGuard);
    if (options & Sound) {
        for (auto & sound : runningSounds) {
            if (sound.getStatus() == sf::Sound::Playing) {
                sound.pause();
            }
        }
    }
    if (options & Music) {
        // currentSong.pause();
    }
}

void SoundController::unpause(int options) {
    std::lock_guard<std::mutex> lk(soundsGuard);
    if (options & Sound) {
        for (auto & sound : runningSounds) {
            if (sound.getStatus() == sf::Sound::Paused) {
                sound.play();
            }
        }
    }
    if (options & Music) {
        // currentSong.play();
    }
}

void SoundController::update() {
    std::lock_guard<std::mutex> lk(soundsGuard);
    if (!soundRequests.empty()) {
        for (const auto req : soundRequests) {
            runningSounds.emplace_back(
                getgEnginePtr()->getResHandler().getSound(req.soundname));
            runningSounds.back().setMinDistance(req.minDistance);
            runningSounds.back().setAttenuation(req.attenuation);
            runningData.push_back({req.source, req.spatialized});
            if (req.spatialized) {
                if (auto sp = req.source.lock()) {
                    const auto pos = sp.get()->getPosition();
                    runningSounds.back().setPosition(pos.x, pos.y, 0.f);
                }
            } else {
                runningSounds.back().setRelativeToListener(true);
            }
            if (req.loop) {
                // For some variety in looped sounds, set random playing offset
                sf::Time loopedTrackLength =
                    runningSounds.back().getBuffer()->getDuration();
                auto randomOffset =
                    calc::rng::random(loopedTrackLength.asMilliseconds());
                sf::Time playingOffset = sf::milliseconds(randomOffset);
                runningSounds.back().setPlayingOffset(playingOffset);
                runningSounds.back().setLoop(true);
            }
            runningSounds.back().play();
        }
        soundRequests.clear();
    }
    while (true) {
        if (runningSounds.size() > 0) {
            if (runningSounds.front().getStatus() == sf::Sound::Stopped) {
                runningSounds.pop_front();
                runningData.pop_front();
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for (auto
             iters = std::make_pair(runningSounds.begin(), runningData.begin());
         iters.first != runningSounds.end(); ++iters.first, ++iters.second) {
        if (iters.second->spatialized) {
            if (auto sp = iters.second->source.lock()) {
                const auto pos = sp.get()->getPosition();
                iters.first->setPosition(pos.x, pos.y, 0.f);
            } else {
                iters.second->spatialized = false;
                iters.first->stop();
            }
        }
    }
}

void SoundController::play(const std::string & name) {
    std::lock_guard<std::mutex> lk(soundsGuard);
    soundRequests.push_back({name, 1.f, 0.f, false, false /*, nullptr not
                                                            convertible to 
                                                            weak_ptr*/});
}

void SoundController::play(const std::string & name,
                           std::shared_ptr<Entity> source, float minDistance,
                           float attenuation, bool loop) {
    std::lock_guard<std::mutex> lk(soundsGuard);
    soundRequests.push_back(
        {name, minDistance, attenuation, true, loop, source});
}
