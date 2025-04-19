#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
//#include <thread>

// Перечисление для типа звука
enum class SoundType {
    Piano,
    Electronic
};

// Структура для представления ноты
struct Note {
    std::string noteName;
    std::chrono::milliseconds startTime;
    std::chrono::milliseconds duration;
};

// Класс для представления клавиши пианино
class PianoKey {
public:
    PianoKey(float x, float y, float width, float height, const sf::Color& color, const std::string& note, sf::Keyboard::Key key, bool isWhite, SoundType soundType)
        : rect(sf::Vector2f(width, height)), soundBuffer(), sound(), noteName(note), keyCode(key), originalColor(color), isWhiteKey(isWhite), currentSoundType(soundType) {
        rect.setPosition(x, y);
        rect.setFillColor(color);
        rect.setOutlineThickness(1);
        rect.setOutlineColor(sf::Color::Black);
        generateSound(noteName, currentSoundType);
    }

    // Метод для отрисовки клавиши
    void draw(sf::RenderWindow& window) {
        // Добавляем тень
        sf::RectangleShape shadow = rect;
        shadow.setFillColor(sf::Color(50, 50, 50, 100)); // Полупрозрачная тень
        shadow.move(5, 5); // Смещение тени
        window.draw(shadow);

        // Добавляем градиент
        sf::RectangleShape gradient = rect;
        sf::VertexArray gradientArray(sf::Quads, 4);
        sf::Color lightColor = originalColor;
        sf::Color darkColor = originalColor - sf::Color(50, 50, 50); // затемнение цвета
        gradientArray[0].position = gradient.getPosition();
        gradientArray[0].color = lightColor;
        gradientArray[1].position = sf::Vector2f(gradient.getPosition().x + gradient.getSize().x, gradient.getPosition().y);
        gradientArray[1].color = lightColor;
        gradientArray[2].position = sf::Vector2f(gradient.getPosition().x + gradient.getSize().x, gradient.getPosition().y + gradient.getSize().y);
        gradientArray[2].color = darkColor;
        gradientArray[3].position = sf::Vector2f(gradient.getPosition().x, gradient.getPosition().y + gradient.getSize().y);
        gradientArray[3].color = darkColor;

        window.draw(gradientArray);

        // Рисуем клавишу поверх градиента
        window.draw(rect);
    }

    // Метод для проверки нажатия клавиши
    bool isKeyPressed() {
        return sf::Keyboard::isKeyPressed(keyCode);
    }

    // Метод для воспроизведения звука
    void play() {
        if (!isPressed) {
            sound.play();
            rect.setFillColor(sf::Color(192, 192, 192));
            isPressed = true;
        }
    }

    // Метод для однократного воспроизведения звука
    void playOnce() {
        sound.play();
    }

    // Метод для остановки воспроизведения звука
    void stop() {
        sound.stop();
        resetColor();
        isPressed = false;
    }

    // Метод для сброса цвета клавиши
    void resetColor() {
        rect.setFillColor(originalColor);
    }

    // Геттер для проверки нажатия клавиши
    bool getIsPressed() const {
        return isPressed;
    }

    // Сеттер для установки состояния нажатия клавиши
    void setIsPressed(bool pressed) {
        isPressed = pressed;
    }

    // Сеттер для установки типа звука клавиши
    void setSoundType(SoundType soundType) {
        currentSoundType = soundType;
        generateSound(noteName, currentSoundType);
    }

    // Геттер для получения кода клавиши
    sf::Keyboard::Key getKeyCode() const {
        return keyCode;
    }

    // Геттер для получения названия ноты
    const std::string& getNoteName() const {
        return noteName;
    }

private:
    bool isPressed = false;
    sf::RectangleShape rect;
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
    std::string noteName;
    sf::Keyboard::Key keyCode;
    sf::Color originalColor;
    bool isWhiteKey;
    SoundType currentSoundType;

    // Метод для генерации звука
    void generateSound(const std::string& note, SoundType soundType) {
        const unsigned sampleRate = 44100; //частота дискретизации
        const unsigned amplitude = 30000; //амплитуда сигнала
        const double twoPi = 6.28318;
        double frequency = getFrequency(note);
        double increment = twoPi * frequency / sampleRate; //угол в радианах, на который изменяется аргумент синусоиды

        std::vector<sf::Int16> samples;
        samples.reserve(sampleRate);

        if (soundType == SoundType::Piano) {
            for (unsigned i = 0; i < sampleRate; ++i) {
                samples.push_back(static_cast<sf::Int16>(amplitude * sin(i * increment)));
            }
        }
        else if (soundType == SoundType::Electronic) {
            for (unsigned i = 0; i < sampleRate; ++i) {
                samples.push_back(static_cast<sf::Int16>(amplitude * (sin(i * increment) + 0.5 * sin(i * increment * 2.0))));
            }//основная частота + 0,5 гармоники
        }

        soundBuffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
        sound.setBuffer(soundBuffer);
        sound.setLoop(false);
    }

    // Метод для получения частоты ноты
    double getFrequency(const std::string& note) {
        if (note == "C3") return 130.81;
        if (note == "C#3") return 138.59;
        if (note == "D3") return 146.83;
        if (note == "D#3") return 155.56;
        if (note == "E3") return 164.81;
        if (note == "F3") return 174.61;
        if (note == "F#3") return 185.00;
        if (note == "G3") return 196.00;
        if (note == "G#3") return 207.65;
        if (note == "A3") return 220.00;
        if (note == "A#3") return 233.08;
        if (note == "B3") return 246.94;
        if (note == "C4") return 261.63;
        if (note == "C#4") return 277.18;
        if (note == "D4") return 293.66;
        if (note == "D#4") return 311.13;
        if (note == "E4") return 329.63;
        if (note == "F4") return 349.23;
        if (note == "F#4") return 369.99;
        if (note == "G4") return 392.00;
        if (note == "G#4") return 415.30;
        if (note == "A4") return 440.00;
        if (note == "A#4") return 466.16;
        if (note == "B4") return 493.88;
        if (note == "C5") return 523.25;
        return 440.00;
    }
};

// Класс для синтезатора
class Synthesizer {
public:
    Synthesizer() : currentSoundType(SoundType::Piano), isRecording(false), melodySaved(false) {
        loadPianoKeys();
        if (!font.loadFromFile("ArialRegular.ttf")) {
            std::cerr << "Error loading font\n";
        }
        soundTypeText.setFont(font);
        soundTypeText.setCharacterSize(24);
        soundTypeText.setFillColor(sf::Color::Black);
        soundTypeText.setPosition(50, 10);
        updateSoundTypeText();

        recordingStatusText.setFont(font);
        recordingStatusText.setCharacterSize(24);
        recordingStatusText.setFillColor(sf::Color::Red);
        recordingStatusText.setPosition(350, 10);
        updateRecordingStatusText();

        melodyStatusText.setFont(font);
        melodyStatusText.setCharacterSize(24);
        melodyStatusText.setFillColor(sf::Color::Green);
        melodyStatusText.setPosition(550, 10);
        updateMelodyStatusText();

        // Add descriptions for keys 3, 4, and 5
        key3Text.setFont(font);
        key3Text.setCharacterSize(24);
        key3Text.setFillColor(sf::Color::Black);
        key3Text.setPosition(900, 50);
        key3Text.setString("3: Play Recorded Melody");

        key4Text.setFont(font);
        key4Text.setCharacterSize(24);
        key4Text.setFillColor(sf::Color::Black);
        key4Text.setPosition(900, 90);
        key4Text.setString("4: Toggle Recording");

        key5Text.setFont(font);
        key5Text.setCharacterSize(24);
        key5Text.setFillColor(sf::Color::Black);
        key5Text.setPosition(900, 130);
        key5Text.setString("5: Toggle Sound Type");

        // Time recording text
        recordingTimeText.setFont(font);
        recordingTimeText.setCharacterSize(24);
        recordingTimeText.setFillColor(sf::Color::Blue);
        recordingTimeText.setPosition(900, 10);
        recordingTimeText.setString("Recording Time: 00:00");
    }

    // Метод для запуска синтезатора
    void run() {
        sf::RenderWindow window(sf::VideoMode(1400, 300), "Synthesizer");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num5) {
                        toggleSoundType();
                        updateSoundTypeText();
                    }
                    else if (event.key.code == sf::Keyboard::Num4) {
                        toggleRecording();
                        updateRecordingStatusText();
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        playRecordedMelody();
                    }
                }
            }

            for (auto& key : pianoKeys) {
                if (key.isKeyPressed()) {
                    if (!key.getIsPressed()) {
                        key.play();
                        if (isRecording) {
                            recordedMelody.push_back({ key.getNoteName(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - recordingStartTime), std::chrono::milliseconds(0) });
                        }
                    }
                }
                else {
                    if (key.getIsPressed()) {
                        if (isRecording) {
                            auto it = std::find_if(recordedMelody.rbegin(), recordedMelody.rend(), [&key](const Note& note) {
                                return note.noteName == key.getNoteName() && note.duration.count() == 0;
                                });
                            if (it != recordedMelody.rend()) {
                                it->duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - recordingStartTime) - it->startTime;
                            }
                        }
                        key.stop();
                    }
                }
            }

            if (isRecording) {
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - recordingStartTime);
                recordingTimeText.setString("Recording Time: " + formatTime(elapsed));
            }

            sf::Color grey(128, 128, 128);
            window.clear(grey);

            for (auto& key : pianoKeys) {
                key.draw(window);
            }

            window.draw(soundTypeText);
            window.draw(recordingStatusText);
            window.draw(recordingTimeText);
            window.draw(melodyStatusText);

            // Draw key descriptions
            window.draw(key3Text);
            window.draw(key4Text);
            window.draw(key5Text);

            window.display();
        }
    }

private:
    std::vector<PianoKey> pianoKeys;
    std::vector<Note> recordedMelody;
    SoundType currentSoundType;
    sf::Font font;
    sf::Text soundTypeText;
    sf::Text recordingStatusText;
    sf::Text melodyStatusText;
    sf::Text key3Text;
    sf::Text key4Text;
    sf::Text key5Text;
    sf::Text recordingTimeText;
    bool isRecording;
    bool melodySaved;
    std::chrono::steady_clock::time_point recordingStartTime;

    // Метод для загрузки клавиш
    void loadPianoKeys() {
        pianoKeys.reserve(26);
        const int numWhiteKeys = 14;
        const int numBlackKeys = 10;
        const float whiteKeyWidth = 60;
        const float whiteKeyHeight = 200;
        const float blackKeyWidth = 40;
        const float blackKeyHeight = 120;
        const float startX = 50;
        const float startY = 50;

        std::vector<std::string> whiteNotes = { "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4" };
        std::vector<sf::Keyboard::Key> whiteKeys = {
            sf::Keyboard::Z, sf::Keyboard::X, sf::Keyboard::C,
            sf::Keyboard::V, sf::Keyboard::B, sf::Keyboard::N,
            sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::S,
            sf::Keyboard::D, sf::Keyboard::F, sf::Keyboard::G,
            sf::Keyboard::H, sf::Keyboard::J
        };
        for (int i = 0; i < numWhiteKeys; ++i) {
            pianoKeys.emplace_back(startX + i * whiteKeyWidth, startY, whiteKeyWidth, whiteKeyHeight, sf::Color::White, whiteNotes[i], whiteKeys[i], true, currentSoundType);
        }

        std::vector<std::string> blackNotes = { "C#3", "D#3", "F#3", "G#3", "A#3", "C#4", "D#4", "F#4", "G#4", "A#4" };
        std::vector<sf::Keyboard::Key> blackKeys = {
            sf::Keyboard::Q, sf::Keyboard::W, sf::Keyboard::E,
            sf::Keyboard::R, sf::Keyboard::T, sf::Keyboard::Y,
            sf::Keyboard::U, sf::Keyboard::I, sf::Keyboard::O,
            sf::Keyboard::P
        };
        std::vector<int> blackKeyPositions = { 0, 1, 3, 4, 5, 7, 8, 10, 11, 12 };
        for (int i = 0; i < numBlackKeys; ++i) {
            pianoKeys.emplace_back(startX + blackKeyPositions[i] * whiteKeyWidth + whiteKeyWidth - blackKeyWidth / 2, startY, blackKeyWidth, blackKeyHeight, sf::Color::Black, blackNotes[i], blackKeys[i], false, currentSoundType);
        }
    }

    // Метод для смены типа звука
    void toggleSoundType() {
        if (currentSoundType == SoundType::Piano) {
            currentSoundType = SoundType::Electronic;
        }
        else {
            currentSoundType = SoundType::Piano;
        }
        for (auto& key : pianoKeys) {
            key.setSoundType(currentSoundType);
        }
    }

    // Метод для обновления текста с типом звука
    void updateSoundTypeText() {
        soundTypeText.setString("Current Sound: " + std::string(currentSoundType == SoundType::Piano ? "Piano" : "Electronic"));
    }

    // Метод для смены состояния записи
    void toggleRecording() {
        if (isRecording) {
            // Завершаем запись
            isRecording = false;

            // Обновляем длительность всех нот
            auto now = std::chrono::steady_clock::now();
            for (auto& note : recordedMelody) {
                if (note.duration.count() == 0) {
                    note.duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - recordingStartTime) - note.startTime;
                }
            }

            // Помечаем мелодию как сохраненную
            melodySaved = true;
        }
        else {
            // Начинаем новую запись
            isRecording = true;
            recordingStartTime = std::chrono::steady_clock::now();
            recordedMelody.clear();
            melodySaved = false; // Помечаем мелодию как несохранённую при начале новой записи
        }
        updateRecordingStatusText();
        updateMelodyStatusText(); // Добавляем обновление статуса мелодии
    }

    // Метод для обновления текста статуса записи
    void updateRecordingStatusText() {
        recordingStatusText.setString(isRecording ? "Recording..." : "Not Recording");
    }

    // Метод для обновления текста статуса мелодии
    void updateMelodyStatusText() {
        melodyStatusText.setString(melodySaved ? "Melody saved" : "Melody not saved");
    }

    // Метод для воспроизведения записанной мелодии
    void playRecordedMelody() {
        if (recordedMelody.empty()) return;

        auto startTime = std::chrono::steady_clock::now();

        for (const auto& note : recordedMelody) {
            auto now = std::chrono::steady_clock::now();
            auto waitDuration = note.startTime - std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
            if (waitDuration.count() > 0) {
                std::this_thread::sleep_for(waitDuration);
            }
            for (auto& key : pianoKeys) {
                if (key.getNoteName() == note.noteName) {
                    key.playOnce();
                    std::this_thread::sleep_for(note.duration);
                    key.stop();
                    break;
                }
            }
        }

        melodySaved = true;
        updateMelodyStatusText();
    }

    // Метод для форматирования времени записи
    std::string formatTime(std::chrono::seconds duration) {
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
        auto seconds = duration - minutes;
        return (minutes.count() < 10 ? "0" : "") + std::to_string(minutes.count()) + ":" + (seconds.count() < 10 ? "0" : "") + std::to_string(seconds.count());
    }
};

// Главная функция
int main() {
    Synthesizer synth;
    synth.run();
    return 0;
};