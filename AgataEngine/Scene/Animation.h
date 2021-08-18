#ifndef AGATA_ANIMATION_H
#define AGATA_ANIMATION_H

#include <vector>
#include "KeyFrames.h"

class Animation {
public:
	Animation() = default;
	Animation(float length);
	Animation(float length, const std::vector<KeyFrame> keyFrames);
	float getLength() const;
	void setLength(float length) { m_Length = length; }
	void addKeyFrame(const KeyFrame& keyFrame);
	std::vector<KeyFrame> getKeyFrames() const;
private:
	std::vector<KeyFrame> m_KeyFrames;
	float m_Length;
};

#endif