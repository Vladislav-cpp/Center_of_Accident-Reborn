#include "Utility.h"
#include <cmath>
#include <iostream>
#include <windows.h>

namespace utility {

std::pair<float, float> normalize(float x, float y) {
    float length = std::hypot(x, y); // краще за sqrt(x*x + y*y)
    if (length == 0.0f) return {0.0f, 0.0f};

    return {x / length, y / length};
}
std::string GetExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string fullPath(buffer);

    // вирізаємо ім'я exe, залишаємо тільки папку
    size_t pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos);
}
void DrawDebugCircle( sf::RenderWindow& window, const sf::Sprite& sprite ) {
    sf::FloatRect local = sprite.getLocalBounds(); // {position, size} в локальних координатах
    sf::Vector2f pos    = sprite.getPosition();
    sf::Vector2f origin = sprite.getOrigin();
    sf::Vector2f scale  = sprite.getScale();

    // Радіус по найбільшій стороні (щоб коло покривало спрайт)
    float radius = (std::max(local.size.x, local.size.y) * std::max(scale.x, scale.y)) * 0.5f;

    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Yellow);
    circle.setOutlineThickness(1.f);

    // Задаємо центр
    // позиція кола — це центр спрайта мінус радіус
    float centerX = pos.x - origin.x * scale.x + local.position.x * scale.x + (local.size.x * scale.x) * 0.5f;
    float centerY = pos.y - origin.y * scale.y + local.position.y * scale.y + (local.size.y * scale.y) * 0.5f;

    circle.setPosition({centerX - radius, centerY - radius});

    // Малюємо
    window.draw(circle);
}

void DrawDebugCapsule( sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color color ) {
	// Трансформація спрайта (позиція, масштаб, поворот, origin)
	sf::Transform transform = sprite.getTransform();
	
	// Локальні границі (SFML 3!)
	sf::FloatRect bounds = sprite.getLocalBounds();
	sf::Vector2f pos = bounds.position; // (x,y)
	sf::Vector2f size = bounds.size;    // (w,h)
	
	// Радіус капсули — половина висоти з урахуванням масштабу
	float radius = (size.y * 0.5f) * sprite.getScale().y;
	
	// Позиції кінцевих точок капсули в ЛОКАЛЬНИХ координатах спрайта:
	sf::Vector2f A(pos.x,           pos.y + size.y * 0.5f);
	sf::Vector2f B(pos.x + size.x,  pos.y + size.y * 0.5f);
	
	// Трансформуємо у світові координати:
	A = transform.transformPoint(A);
	B = transform.transformPoint(B);
	
	// Малюємо центральну лінію
	sf::Vertex line[] = {
		{ A, color },
		{ B, color }
	};
	window.draw(line, 2, sf::PrimitiveType::Lines);
	
	// Підготовка кола
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(color);
	circle.setOrigin({radius, radius});
	
	// Коло в точці A
	circle.setPosition(A);
	window.draw(circle);
	
	// Коло в точці B
	circle.setPosition(B);
	window.draw(circle);
	
	// Бічні ребра (для повного «капсульного» вигляду)
	sf::Vector2f d = B - A;
	float len = std::sqrt(d.x*d.x + d.y*d.y);
	if (len <= 0.001f) return; // уникнення ділення на нуль
	d /= len;
	
	// Нормаль до лінії AB
	sf::Vector2f n(-d.y, d.x);
	
	// Крайні точки з обох сторін
	sf::Vector2f A_left  = A + n * radius;
	sf::Vector2f A_right = A - n * radius;
	sf::Vector2f B_left  = B + n * radius;
	sf::Vector2f B_right = B - n * radius;
	
	sf::Vertex side1[] = { {A_left, color}, {B_left, color} };
	sf::Vertex side2[] = { {A_right, color}, {B_right, color} };
	
	window.draw(side1, 2, sf::PrimitiveType::Lines);
	window.draw(side2, 2, sf::PrimitiveType::Lines);
}



void DrawDebugRect(sf::RenderWindow& window, const sf::Sprite& sprite) {
    sf::FloatRect local = sprite.getLocalBounds(); // { pos, size } в *локальних координатах*
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f origin = sprite.getOrigin();
    sf::Vector2f scale = sprite.getScale();

    float left = pos.x - origin.x * scale.x + local.position.x * scale.x;
    float top  = pos.y - origin.y * scale.y + local.position.y * scale.y;

    float width  = local.size.x * scale.x;
    float height = local.size.y * scale.y;

    sf::RectangleShape rect;
    rect.setPosition({left, top});
    rect.setSize({ width, height });
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1.f);

    window.draw(rect);
}

void DrawDebug( sf::RenderWindow& window, const sf::Sprite& sprite, const IStaticStatsBase& stat ) {

	if(stat.objectName == "Ufo") DrawDebugCircle(window, sprite);
	else if(stat.objectName == "Chuck") DrawDebugCapsule(window, sprite);
	else if(stat.objectName == "Dust") DrawDebugRect(window, sprite);
	else  {
		std::cout << "DrawDebug empty";
		__debugbreak();
	}
}
sf::Vector2f ClampToMap(const sf::Vector2f& pos, float viewWidth, float viewHeight, float mapWidth, float mapHeight) {
    float halfWidth = viewWidth / 2.f;
    float halfHeight = viewHeight / 2.f;

    sf::Vector2f clamped = pos;

    if (clamped.x < halfWidth) clamped.x = halfWidth;
    if (clamped.x > mapWidth - halfWidth) clamped.x = mapWidth - halfWidth;
    if (clamped.y < halfHeight) clamped.y = halfHeight;
    if (clamped.y > mapHeight - halfHeight) clamped.y = mapHeight - halfHeight;

    return clamped;
}

//
// 1. Отримати 4 глобальні вершини спрайта
//
std::array<sf::Vector2f, 4> GetSpriteGlobalQuad(const sf::Sprite& spr)
{
    sf::FloatRect b = spr.getLocalBounds();

    // Локальні вершини (SFML 3)
    std::array<sf::Vector2f, 4> pts = {
        sf::Vector2f{ b.position.x,                   b.position.y },
        sf::Vector2f{ b.position.x + b.size.x,        b.position.y },
        sf::Vector2f{ b.position.x + b.size.x,        b.position.y + b.size.y },
        sf::Vector2f{ b.position.x,                   b.position.y + b.size.y }
    };

    // Переводимо у глобальні координати з урахуванням:
    // transform = position + origin + scale + rotation
    for (auto& p : pts)
        p = spr.getTransform().transformPoint(p);

    return pts;
}

//
// 2. Коло (c2Circle)
//   - центр = середнє всіх точок
//   - радіус = половина мінімальної сторони
//
c2Circle GetCircle(const sf::Sprite& spr)
{
    auto q = GetSpriteGlobalQuad(spr);

    sf::Vector2f center{
        (q[0].x + q[1].x + q[2].x + q[3].x) / 4.f,
        (q[0].y + q[1].y + q[2].y + q[3].y) / 4.f
    };

    float w = std::hypot(q[1].x - q[0].x, q[1].y - q[0].y);
    float h = std::hypot(q[3].x - q[0].x, q[3].y - q[0].y);

    c2Circle c;
    c.p = c2V(center.x, center.y);
    c.r = std::min(w, h) * 0.5f;

    return c;
}

//
// 3. AABB (axis-aligned bounding box)
//
c2AABB GetAABB(const sf::Sprite& spr)
{
    auto q = GetSpriteGlobalQuad(spr);

    float minX = std::min({ q[0].x, q[1].x, q[2].x, q[3].x });
    float maxX = std::max({ q[0].x, q[1].x, q[2].x, q[3].x });

    float minY = std::min({ q[0].y, q[1].y, q[2].y, q[3].y });
    float maxY = std::max({ q[0].y, q[1].y, q[2].y, q[3].y });

    c2AABB box;
    box.min = c2V(minX, minY);
    box.max = c2V(maxX, maxY);

    return box;
}

//
// 4. Капсула (c2Capsule)
//   — по довгій стороні спрайта
//
c2Capsule GetCapsule(const sf::Sprite& spr)
{
    // 1. Реальні розміри текстури
    const sf::Texture* tex = &spr.getTexture();
    float texW = tex->getSize().x * spr.getScale().x;
    float texH = tex->getSize().y * spr.getScale().y;

    // 2. Центр у глобальних координатах
    sf::Vector2f center = spr.getTransform().transformPoint(
        sf::Vector2f(tex->getSize().x * 0.5f, tex->getSize().y * 0.5f)
    );

    // 3. Напрямок довгої осі спрайта
    sf::Vector2f axisDir;

    bool horizontal = texW >= texH;

    if (horizontal)
        axisDir = spr.getTransform().transformPoint({1.f, 0.f}) -
                  spr.getTransform().transformPoint({0.f, 0.f});
    else
        axisDir = spr.getTransform().transformPoint({0.f, 1.f}) -
                  spr.getTransform().transformPoint({0.f, 0.f});

    // нормалізація
    float len = std::sqrt(axisDir.x*axisDir.x + axisDir.y*axisDir.y);
    axisDir /= len;

    // 4. Довжина прямої частини
    float halfLong  = (horizontal ? texW : texH) * 0.5f;
    float radius    = (horizontal ? texH : texW) * 0.5f;

    // 5. Точки капсули
    sf::Vector2f A = center - axisDir * halfLong;
    sf::Vector2f B = center + axisDir * halfLong;

    c2Capsule cap;
    cap.a = c2V(A.x, A.y);
    cap.b = c2V(B.x, B.y);
    cap.r = radius;

    return cap;
}



//
// 5. Полігон (4 вершини)
//
c2Poly GetQuadPoly(const sf::Sprite& spr)
{
    auto q = GetSpriteGlobalQuad(spr);

    c2Poly p;
    p.count = 4;

    for (int i = 0; i < 4; i++)
        p.verts[i] = c2V(q[i].x, q[i].y);

    c2MakePoly(&p);
    return p;
}

//
// 6. Полігон (трикутник)
//
// Наприклад: вершини 0-1-2
//
c2Poly GetTrianglePoly(const sf::Sprite& spr)
{
    auto q = GetSpriteGlobalQuad(spr);

    c2Poly p;
    p.count = 3;

    p.verts[0] = c2V(q[0].x, q[0].y);
    p.verts[1] = c2V(q[1].x, q[1].y);
    p.verts[2] = c2V(q[2].x, q[2].y);

    c2MakePoly(&p);
    return p;
}


} // namespace utility