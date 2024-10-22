#pragma once
#include "Tools.hpp"
#include "StateManager.hpp"
#include "RessourcesManager.hpp"


class SSprite
{
public:
	SSprite() { m_state = State::RTNULL; };
	SSprite(std::string Name, State _state, std::string Path)
	{
		m_name = Name;
		m_state = _state;
		
		if (m_texture.loadFromFile(Path))
		{
			m_sprite.setTexture(m_texture); // need to redo it beacause the address is moved when pushback
		}
		else
		{
			std::cout << "ERROR: Sprite : " << m_name << " didn't load" << std::endl;
		}
	};
	~SSprite() {};
	void init(std::string Name, State _state, std::string Path) 
	{
		m_name = Name;
		m_state = _state;

		if (m_texture.loadFromFile(Path))
		{
			m_texture.setSmooth(true);
			m_sprite.setTexture(m_texture); // need to redo it beacause the address is moved when pushback
		}
		else
		{
			std::cout << "ERROR: Sprite : " << m_name << " didn't load" << std::endl;
		}
	};

	std::string getName() const { return m_name; };
	State getState() const { return m_state; };
	sf::Sprite& getSprite() { return m_sprite; };
	sf::Texture& getTexture() { return m_texture; };

	bool operator == (const SSprite& s) const { return m_name == s.m_name && m_state == s.m_state; };
	bool operator != (const SSprite& s) const { return !operator==(s); };

private:
	std::string m_name;
	State m_state;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

};


extern std::list<SSprite*> SpriteList;

void LoadSprite(State _state);
sf::Sprite& getSprite(std::string Name);
sf::Texture& getTexture(std::string Name);
void RemoveAllSprites();
void RemoveStateSprites(State _state);