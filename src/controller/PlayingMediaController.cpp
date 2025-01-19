
#include "view/PlayingView.h"


bool isSDLInitialized = false;
Mix_Music* currentMusic = nullptr;

void initializeSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
        return;
    }
    isSDLInitialized = true;
}

void cleanupSDL() {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

std::atomic<bool> isRunning(true);
std::atomic<bool> isPlaying(true);
std::atomic<int> currentTime(0);
std::atomic<int> totalTime(0);
std::mutex mediaMutex;
std::thread updateThread;
std::mutex inputMutex;
std::condition_variable inputCondition;

void PlayingMediaController::handleAction(int choice) {
    switch (choice) {
        case 1: { // Play/Pause
            if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                isPlaying = true;
                std::cout << "Resuming music...\n";
            } else {
                Mix_PauseMusic();
                isPlaying = false;
                std::cout << "Pausing music...\n";
            }
            break;
        }
        case 2: skipToNext(); break;
        case 3: skipToPrevious(); break;
        case 4: {
            int newVolume;
            std::cout << "Enter new volume (0-100): ";
            std::cin >> newVolume;
            adjustVolume(newVolume);
            break;
        }
        case 0: {
            isRunning = false;
            std::cout << "\nExiting Playing Menu...\n";
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                std::cerr << "Error: MediaFileController is not available!" << std::endl;
                break;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            mediaFileController->scanAndDisplayMedia();
        }
        default:
            std::cerr << "Invalid choice.\n";
            break;
    }
}


void PlayingMediaController::adjustVolume(int level) {
    if (level < 0 || level > 100) {
        std::cerr << "Volume level must be between 0 and 100.\n";
        return;
    }

    volume = level;
    Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);

    std::cout << "Volume set to: " << volume << "%\n";
}


void PlayingMediaController::stop() {
    if (!isPlaying) return;

    isRunning = false;

    // Đợi thread cũ kết thúc
        if (updateThread.joinable() && std::this_thread::get_id() != updateThread.get_id()) {
        updateThread.join();
    }

    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }

    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (playingView) {
        playingView->clearView();
        playingView->displayPlayingView("No Media", 0, volume, 0);
    }

    isPlaying = false;
}

void PlayingMediaController::skipToNext() {
    stop(); // Dừng phát nhạc hiện tại

    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex + 1) % currentPlaylist->size();
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            std::cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.front();
        } else {
            auto it = std::find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end()) {
                ++it;
                currentMediaFile = (it != mediaFiles.end()) ? &(*it) : &mediaFiles.front();
            } else {
                currentMediaFile = &mediaFiles.front();
            }
        }
    }

    playMediaFile(currentMediaFile); // Phát nhạc mới
}

void PlayingMediaController::skipToPrevious() {
    stop(); // Dừng phát nhạc hiện tại

    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex == 0) ? currentPlaylist->size() - 1 : currentPlaylistIndex - 1;
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            std::cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.back();
        } else {
            auto it = std::find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end() && it != mediaFiles.begin()) {
                --it;
                currentMediaFile = &(*it);
            } else {
                currentMediaFile = &mediaFiles.back();
            }
        }
    }

    playMediaFile(currentMediaFile); // Phát nhạc mới
}



void PlayingMediaController::playMediaFile(MediaFile* mediaFile) {
    if (!mediaFile) {
        std::cerr << "No media file provided.\n";
        return;
    }

    stop(); // Dừng nhạc hiện tại trước khi phát bài mới
    isRunning = true;

    if (!isSDLInitialized) {
        initializeSDL();
        if (!isSDLInitialized) {
            std::cerr << "Failed to initialize SDL.\n";
            return;
        }
    }

    currentMusic = Mix_LoadMUS(mediaFile->getPath().c_str());
    if (!currentMusic) {
        std::cerr << "Failed to load music file: " << mediaFile->getPath() << " (" << Mix_GetError() << ")\n";
        return;
    }

    if (Mix_PlayMusic(currentMusic, 1) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << "\n";
        return;
    }

    currentMediaFile = mediaFile;
    isPlaying = true;

    auto fileRef = TagLib::FileRef(mediaFile->getPath().c_str());
    totalTime = fileRef.audioProperties() ? fileRef.audioProperties()->length() : 0;
    currentTime = 0;

    // Cập nhật giao diện người dùng
    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playingView) {
        std::cerr << "PlayingView not available.\n";
        return;
    }

    // Khởi động luồng cập nhật
    updateThread = std::thread([this, playingView]() {
        while (isRunning) {
            {
                std::lock_guard<std::mutex> lock(mediaMutex);
                playingView->clearView();
                playingView->displayPlayingView(
                    currentMediaFile->getName(),
                    totalTime.load(),
                    volume,
                    currentTime.load()
                );
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (isPlaying) {
                ++currentTime;
                if (currentTime >= totalTime) {
                    std::cout << "\nPlayback finished. Skipping to next track...\n";
                    isRunning = false; // Ngăn vòng lặp tiếp tục
                    std::thread([this]() {
                        skipToNext(); // Chuyển bài trong thread riêng
                    }).detach();
                    break;
                }
            }
        }
    });
}



void PlayingMediaController::playPlaylist(std::vector<MediaFile>& playlist) {
    currentPlaylist = &playlist;
    currentPlaylistIndex = 0;
    if (!playlist.empty()) {
        currentMediaFile = &playlist[currentPlaylistIndex];
        playMediaFile(currentMediaFile);
    } else {
        std::cerr << "Playlist is empty.\n";
    }
}

