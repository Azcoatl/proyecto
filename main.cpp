#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

constexpr float SCALE = 30.0f;

// Clase Cube
class Cube {
public:
    Cube(b2World &world, float x, float y, int state, float sizex, float sizey, float shapex, float shapey, sf::Color color) {
        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);
        switch (state) {
            case 0:
                bodydef.type = b2_staticBody;
                break;
            case 1:
                bodydef.type = b2_dynamicBody;
                break;
        }
        body = world.CreateBody(&bodydef);

        b2PolygonShape shape;
        shape.SetAsBox((shapex / 2) / SCALE, (shapey / 2) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        body->CreateFixture(&fixturedef);

        box.setSize(sf::Vector2f(sizex, sizey));
        box.setFillColor(color);
        box.setOrigin(shapex / 2, shapey / 2);
    }

    void update() {
        box.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);
        box.setRotation(body->GetAngle() * 180 / b2_pi);
    }

    void draw(sf::RenderWindow &win) const {
        win.draw(box);
    }

    void resetPosition() {
        body->SetTransform(b2Vec2(0, 0), body->GetAngle());
    }

    void stop() {
        body->SetLinearVelocity(b2Vec2(0, 0));
    }

    b2Body *getBody() {
        return body;
    }

private:
    sf::RectangleShape box;
    b2Body *body;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");

    // Se establece la gravedad en el mundo y su fuerza
    b2Vec2 gravity(0.0f, 0.02f);
    b2World world(gravity);

    // Objetos
    Cube cube3(world, 200, 300, 1, 10.0f, 10.0f, 10.0f, 10.0f,sf::Color::Cyan);
    Cube plataforma1(world, 200, 311, 0, 20.0f, 10.0f, 20.0f, 10.0f,sf::Color::Cyan);
    Cube cube9(world, 800, 520, 1, 10.0f, 10.0f, 10.0f, 10.0f,sf::Color::Cyan);
    Cube plataforma4(world, 800, 531, 0, 20.0f, 10.0f, 20.0f, 10.0f,sf::Color::Cyan);
    Cube plataforma3(world, 550, 600, 0, 300.0f, 10.0f, 300.0f, 10.0f, sf::Color::Cyan);
    Cube domino1(world, 700, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino2(world, 650, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino3(world, 600, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino4(world, 550, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino5(world, 500, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino6(world, 450, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);
    Cube domino7(world, 400, 550, 1, 10.0f, 50.0f, 10.0f, 50.0f, sf::Color::White);


    // Articulacion de Rotacion
    // Objetos en uso
    Cube cube1(world, 200, 200, 0, 10.0f, 10.0f, 10.0f, 10.0f, sf::Color::Yellow);
    Cube cube2(world, 200, 100, 1, 10.0f, 10.0f,10.0f, 10.0f, sf::Color::Yellow);

    b2RevoluteJointDef jointDefRevolute;
    jointDefRevolute.Initialize(cube2.getBody(), cube1.getBody(), cube1.getBody()->GetWorldCenter());
    jointDefRevolute.enableMotor = true;
    jointDefRevolute.maxMotorTorque = 1.0f;
    jointDefRevolute.motorSpeed = 0.39f;
    world.CreateJoint(&jointDefRevolute);

    // Articulacion Prismatica
    // Objetos en uso
    Cube cube5(world, 480, 290, 0, 70.0f, 10.0f, 70.0f, 10.0f, sf::Color::Green);
    Cube cube6(world, 540, 280, 1, 10.0f, 10.0f, 10.0f, 10.0f, sf::Color::Green);

    b2PrismaticJointDef jointDefPrismatic;
    jointDefPrismatic.Initialize(cube6.getBody(), cube5.getBody(), cube6.getBody()->GetWorldCenter(), b2Vec2(1.0f, 0.0f));
    jointDefPrismatic.lowerTranslation = -9.0f;
    jointDefPrismatic.upperTranslation = 1.0f;
    jointDefPrismatic.enableLimit = true;
    world.CreateJoint(&jointDefPrismatic);

    // Articulacion de Distancia
    // Objetos en uso
    Cube cube7(world, 800, 280, 1, 10.0f, 10.0f, 10.0f, 10.0f, sf::Color::Blue);
    Cube cube8(world, 800, 400, 0, 10.0f, 10.0f, 10.0f, 10.0f,sf::Color::Blue);
    Cube plataforma2(world, 800, 400, 0, 10.0f, 10.0f,10.0f, 10.0f, sf::Color::Blue);

    b2DistanceJointDef jointDefDistance;
    jointDefDistance.Initialize(cube7.getBody(), cube8.getBody(), cube7.getBody()->GetWorldCenter(), cube8.getBody()->GetWorldCenter());
    float frequencyHz = 30.0f;
    float dampingRatio = 0.1f;
    b2LinearStiffness(jointDefDistance.stiffness, jointDefDistance.damping, frequencyHz, dampingRatio, jointDefDistance.bodyA, jointDefDistance.bodyB);
    world.CreateJoint(&jointDefDistance);

    // Articulación de Polea
    // Objetos en uso
    Cube cube11(world, 170, 320, 1, 10.0f, 20.0f, 10.0f, 20.0f, sf::Color::Red);
    Cube cube12(world, 350, 600, 1, 50.0f, 10.0f, 50.0f, 10.0f, sf::Color::Red);
    Cube cube13(world, 170, 350, 0, 10.0f, 10.0f, 10.0f, 10.0f, sf::Color::Red);
    Cube cube14(world, 350, 650, 0, 50.0f, 10.0f, 50.0f, 10.0f, sf::Color::Red);

    b2PulleyJointDef jointPulley;
    jointPulley.Initialize(cube11.getBody(),cube12.getBody(),cube13.getBody()->GetWorldCenter(),cube14.getBody()->GetWorldCenter(),cube11.getBody()->GetWorldCenter(),cube12.getBody()->GetWorldCenter(), 1);
    world.CreateJoint(&jointPulley);

    // Articulacion prismatica para la polea, sirve para que no se muevan de lugar en el eje y
    b2PrismaticJointDef jointDefPrismatic2;
    jointDefPrismatic2.Initialize(cube13.getBody(), cube11.getBody(), cube13.getBody()->GetWorldCenter(), b2Vec2(0.0f, 1.0f));
    jointDefPrismatic2.lowerTranslation = -1.0f;
    jointDefPrismatic2.upperTranslation = 1.0f;
    jointDefPrismatic2.enableLimit = true;
    world.CreateJoint(&jointDefPrismatic2);

    b2PrismaticJointDef jointDefPrismatic3;
    jointDefPrismatic3.Initialize(cube14.getBody(), cube12.getBody(), cube14.getBody()->GetWorldCenter(), b2Vec2(0.0f, 1.0f));
    jointDefPrismatic3.lowerTranslation = -1.0f;
    jointDefPrismatic3.upperTranslation = 1.0f;
    jointDefPrismatic3.enableLimit = true;
    world.CreateJoint(&jointDefPrismatic3);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // The Z key event to close the window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                window.close();
                return 0; // return from the main function, effectively ending the program
            }
        }

        // Aqui se actualizan las figuras en la ventana
        world.Step(1 / 60.f, 8, 3);
        cube1.update();
        cube2.update();
        cube3.update();
        plataforma1.update();
        cube5.update();
        cube6.update();
        cube7.update();
        cube8.update();
        cube9.update();
        plataforma4.update();
        cube11.update();
        cube12.update();
        cube13.update();
        cube14.update();
        plataforma3.update();
        domino1.update();
        domino2.update();
        domino3.update();
        domino4.update();
        domino5.update();
        domino6.update();
        domino7.update();

        // Aqui se dibujan las figuras en la ventana
        window.clear();
        cube1.draw(window);
        cube2.draw(window);
        cube3.draw(window);
        plataforma1.draw(window);
        cube5.draw(window);
        cube6.draw(window);
        cube7.draw(window);
        cube8.draw(window);
        cube9.draw(window);
        plataforma4.draw(window);
        cube11.draw(window);
        cube12.draw(window);
        cube13.draw(window);
        cube14.draw(window);
        plataforma3.draw(window);
        domino1.draw(window);
        domino2.draw(window);
        domino3.draw(window);
        domino4.draw(window);
        domino5.draw(window);
        domino6.draw(window);
        domino7.draw(window);
        window.display();
    }
    return 0;
}
