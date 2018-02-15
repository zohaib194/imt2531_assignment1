#include "glm/glm.hpp"

template <class T>
glm::vec4 collision(T obj)
{
    int i = (obj.position.y / (2 / w.size.y)) - 1, j = (obj.position.x / (2 / w.size.x)) + 1;
    return glm::vec4((w.map[int((j    ) * w.size.x + (i - 1))].z == 1),
                     (w.map[int((j + 1) * w.size.x + (i    ))].z == 1),
                     (w.map[int((j    ) * w.size.x + (i + 1))].z == 1),
                     (w.map[int((j - 1) * w.size.x + (i    ))].z == 1));
    // TODO: 
    // - use found i and j to look up actual world position within w.map, and check agains pacmans entire body collides with something, not just if the next tile is a wall
    // - Make function take in object to check agains, maybe also the xy size of the object
}