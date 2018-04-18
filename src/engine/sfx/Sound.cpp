#include "engine\sfx\Sound.h"

Source::Source(std::string p_sourceName)
{
	m_name = p_sourceName;
	alGenSources(1, &m_sourceId);
	setPosition({});
	setVelocity({});
}

void Source::setSound(Buffer p_buffer)
{
	m_buffer = p_buffer;
	alSourcei(m_sourceId, AL_BUFFER, m_buffer.getId());
}

void Source::setPosition(Vector3<GLfloat> p_pos)
{
	m_pos = p_pos;
	alSource3f(m_sourceId, AL_POSITION, m_pos.x, m_pos.y, m_pos.z);
}
void Source::setVelocity(Vector3<GLfloat> p_vel)
{
	m_vel = p_vel;
	alSource3f(m_sourceId, AL_VELOCITY, m_vel.x, m_vel.y, m_vel.z);
}

void Source::play()
{
	alSourcePlay(m_sourceId);
}
void Source::play(Vector3<GLfloat> p_pos)
{
	m_pos = p_pos;
	alSource3f(m_sourceId, AL_POSITION, m_pos.x, m_pos.y, m_pos.z);
	alSourcePlay(m_sourceId);
}

void Source::play2d()
{
	Vector3<GLfloat> pos;
	alGetListenerfv(AL_POSITION, &pos.x);
	setPosition(pos);
	alSourcePlay(m_sourceId);
	setPosition(m_pos);
}



Buffer::Buffer(std::string p_fileName)
{
	if(p_fileName != "")
	{
		m_bufferId = alutCreateBufferFromFile(("res\\sound\\" + p_fileName).c_str());
	}
}



bool Sound::init()
{
	alutInitWithoutContext(NULL, NULL);

	m_device = alcOpenDevice(NULL);
	if(m_device) {
		m_context = alcCreateContext(m_device, NULL);
		alcMakeContextCurrent(m_context);
	}

	bool _bEAX = alIsExtensionPresent("EAX2.0");

	return true;
}

void Sound::terminate()
{
	m_context = alcGetCurrentContext();
	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);

	alutExit();
}

void Sound::setListenPosition(Vector3<GLfloat> p_pos)
{
	alListener3f(AL_POSITION, p_pos.x, p_pos.y, p_pos.z);
}
void Sound::setListenVelocity(Vector3<GLfloat> p_vel)
{
	alListener3f(AL_VELOCITY, p_vel.x, p_vel.y, p_vel.z);
}
void Sound::setListenOrientation(Vector3<GLfloat> p_lookAt)
{
	ALfloat ori[] = {p_lookAt.x, p_lookAt.y, p_lookAt.z, 0, 1, 0};
	alListenerfv(AL_ORIENTATION, ori);
}
