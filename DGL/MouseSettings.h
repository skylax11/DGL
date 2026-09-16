#pragma once
struct MouseSettings
{
public:
    float m_sensitivity;
    float m_maxPitch;
    float m_minPitch;

    MouseSettings(float sensitivity, float maxPitch, float minPitch);
    ~MouseSettings();
};

