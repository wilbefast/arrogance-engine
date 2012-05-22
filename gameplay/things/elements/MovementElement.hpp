#ifndef MOVEMENTELEMENT_HPP_INCLUDED
#define MOVEMENTELEMENT_HPP_INCLUDED

#include "ThingElement.hpp"
#include "../../../scenes/GameState.hpp"

class MovementElement : public ThingElement
{
    /// CONSTANTS
    private:
    static const float DEFAULT_SPEED_MIN;

    /// ATTRIBUTES
    private:
    fV2 speed;
    fV2 previous_position;
    float speed_scalar; // scalars stored on speed change for quick access
    float angle;
    float speed_max;
    float speed_min;
    float friction;

    /// METHODS
    public:
    // constructors, destructors
    MovementElement(Thing* init_owner, float init_speed_max,
                    float init_friction = 1,
                    float init_speed_min = DEFAULT_SPEED_MIN);
    ~MovementElement();
    // main methods
    void springTowards(fV2 target_position, float spring_constant);
    void add_springTowards(fV2 target_position, float spring_constant);
    void moveTowards(fV2 target_position, float speed);
    void add_moveTowards(fV2 target_position, float speed);
    void add_speedScalar(float speed);
    void bounce(iV2 collision_side);
    int update(GameState* context);
    // modification
    void setSpeed_scalar(float new_speed_scalar);
    void setSpeed(fV2 new_speed);
    void addSpeed(fV2 force);
    void setSpeedMax(float _max_speed);
    // query
    fV2 getSpeed() const;
    fV2 getPrevPos() const;
    float getSpeed_scalar() const;
    float getAngle() const;

};

#endif // MOVEMENTELEMENT_HPP_INCLUDED
