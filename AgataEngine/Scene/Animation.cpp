#include "Animation.h"

Animation::Animation(float length, const std::vector<KeyFrame> keyFrames) {

	m_Length = length;
	m_KeyFrames = keyFrames;

}

Animation::Animation(float length) {

	m_Length = length;

}

void Animation::addKeyFrame(const KeyFrame& keyFrame) {

	m_KeyFrames.push_back(keyFrame);

}

float Animation::getLength() const {

	return m_Length;

}

std::vector<KeyFrame> Animation::getKeyFrames() const {

	return m_KeyFrames;

}