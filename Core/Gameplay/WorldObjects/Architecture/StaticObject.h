#pragma once

#include <SFML/System/Vector2.hpp>
#include "IPlayerController.h"
#include "UIElement.h"
#include <memory>
#include "IStats.h"
#include <functional>

class StaticObject : public std::enable_shared_from_this<StaticObject> {
	using CoordCallback = std::function<void(const sf::Vector2f&)>;

	public:
	virtual ~StaticObject() = default;
	StaticObject() = default;

	public:
	int ID();
	sf::Vector2f Coord();

	public:
	virtual void SetCoord(sf::Vector2f coord);
	virtual void SetID(int id);
	virtual void AddUI(UIType type, UIElement* uiElement);
	virtual void SetStats(IStaticStatsBase& staticSt, IDynamicStatsBase& dynamicSt) { 
		m_pStaticStats = staticSt.clone(); 
		m_pDynamicStats = dynamicSt.clone();
	}
	
	virtual void ChangeUIState(UIState newState);
	virtual void ChangeUIState(UIType type, UIState newState);

	public:
	void SetController(std::unique_ptr<IPlayerController> controller);
	std::unique_ptr<IPlayerController>& GetController() { return m_pController; };


	public:
	virtual void UpdateViewCoord();

	public:
	virtual const std::unique_ptr<IStaticStatsBase>& GetPrototpe() { return m_pStaticStats; };
	virtual UIElement* GetUI() { return GetUI(UIType::Main); };
	virtual UIElement* GetUI(UIType type);
	virtual const IStaticStatsBase& GetStaticStats() { return *m_pStaticStats.get(); }
	virtual const IDynamicStatsBase& GetDynamicStats() { return *m_pDynamicStats.get(); }

	public:
	virtual const void Draw(sf::RenderWindow& window, float time);

	public:
	void SubscribeOnCoordChange(CoordCallback cb);

	protected:
	void OnCoordChange();

	protected:
	std::unique_ptr<IStaticStatsBase> m_pStaticStats{};
	std::unique_ptr<IDynamicStatsBase> m_pDynamicStats{};
	std::unordered_map<UIType, UIElement*> m_vUIViews;

	#define m_iID m_pDynamicStats->m_iID
	sf::Vector2f m_vCoord{0.f, 0.f};
	//#define m_vCoord m_pDynamicStats->m_vCoord

//	TODO Macros
//	decltype(IDynamicStatsBase::m_iID)& m_iID = m_pDynamicStatsSO->m_iID;
//	DYNAMIC_STATS_FIELDS(m_pDynamicStatsSO);

	protected:
	std::unique_ptr<IPlayerController> m_pController;

	private:
	std::vector<CoordCallback> m_vCoordListeners;
};

