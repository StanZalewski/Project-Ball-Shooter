#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <iterator>

using namespace std;

//Game_wall and Game_menu class declaration from Game class
class Game_wall;
class Game_menu;

// Game class inicialization for later use by Game_wall
class Game
{
private:
    int score_;

public:
    Game(int score_) : score_(score_) {}

    int getScore() const
    {
        return score_;
    }
    void setScore(int score)
    {
        score_ = score;
    }
    
       // texture func - input and store all textures in one vector
    std::vector<sf::Texture> texture() {
    std::vector<sf::Texture> textures;
    sf::Texture vertical;
    if (!vertical.loadFromFile("textures/vertical.png")) {
        std::cout << "Texture VERTICAL fail to load" << std::endl;
    }
    textures.push_back(vertical);

    sf::Texture horizontal;
    if (!horizontal.loadFromFile("textures/horizontal.png")) {
        std::cout << "Texture HORIZONTAL fail to load" << std::endl;
    }
    textures.push_back(horizontal);

    sf::Texture cross;
    if (!cross.loadFromFile("textures/cross.png")) {
        std::cout << "Texture Cross fail to load" << std::endl;
    }
    textures.push_back(cross);

    sf::Texture cross_L2R;
    if (!cross_L2R.loadFromFile("textures/cross_L2R.png")) {
        std::cout << "Texture Cross_L2R fail to load" << std::endl;
    }
    textures.push_back(cross_L2R);

    sf::Texture cross_R2L;
    if (!cross_R2L.loadFromFile("textures/cross_R2L.png")) {
        std::cout << "Texture Cross_R2L fail to load" << std::endl;
    }
    textures.push_back(cross_R2L);

    sf::Texture all_direct;
    if (!all_direct.loadFromFile("textures/all_direct.png")) {
        std::cout << "Texture ALL_DIRECT fail to load" << std::endl;
    }
    textures.push_back(all_direct);

    return textures;
}

    void start(Game_wall &objects_wall, Game_menu &objects_menu, int score);
    void dis(Game_wall &objects_wall, Game_menu &objects_menu, sf::RenderWindow &window);
};

//Vector for string specyfic colors 
std::vector<sf::Color> vec_color() 
    {
        std::vector<sf::Color> vec_color_ ;
        vec_color_.push_back(sf::Color(0, 0, 0));
        vec_color_.push_back(sf::Color(209, 109, 106));
        vec_color_.push_back(sf::Color(157, 194, 132));
        vec_color_.push_back(sf::Color(120, 157, 229));
        vec_color_.push_back(sf::Color(222, 207, 105));
        vec_color_.push_back(sf::Color(61,39,108));
        vec_color_.push_back(sf::Color(107, 34, 70));
        return vec_color_;
    }


//Object class inicialization with class Balls inheriting form Object and Bomb inheriting form Balls

class Object : public sf::CircleShape {
protected:
    sf::Vector2f position_;
    sf::Color color_;

public:
    Object(const sf::Vector2f &size_, const sf::Vector2f &position_, sf::Color color_)
        : sf::CircleShape(size_.x), position_(position_)
    {
        setFillColor(color_); // Set the fill color of the circle shape
        setPosition(position_); // Set the position of the circle shape
    }

    sf::Vector2f getPosition() const
    {
        return position_;
    }

    void setPosition(const sf::Vector2f &position)
    {
        position_ = position;
        sf::CircleShape::setPosition(position); // Set the position of the circle shape
    }

    int getSize() const
    {
        return static_cast<int>(getRadius());
    }

    void setSize(int size)
    {
        setRadius(static_cast<float>(size));
    }

    sf::Color getColor() const
    {
        return color_;
    }

    void setColor(sf::Color color)
    {
        color_ = color;
        setFillColor(color_); // Set the fill color of the circle shape
    }

};


class Balls : public Object
{
protected:
    bool ifBomb_;

public:
    Balls(const sf::Vector2f &size_, const sf::Vector2f &position_, sf::Color color_, bool ifBomb)
    : Object(size_, position_, color_), ifBomb_(ifBomb) {}

    bool getifBomb() const
    {
        return ifBomb_;
    };
};

class Bomb : public Balls
{
protected:
    int destructions_;

public:
    Bomb(const sf::Vector2f &size_, const sf::Vector2f &position_, sf::Color color_, bool ifBomb_, int destructions_)
    : Balls(size_, position_, color_, ifBomb_), destructions_(destructions_) {}

    int getDesctructions(){
        return destructions_;
    }

};

class Game_wall
{
private:
    friend class Game;
    std::vector<std::vector<Object *> > wall_;
    int width_ ;
    int height_ ;

public:
   Game_wall(const int width_, const int height_) : width_(width_), height_(height_)
{
    initializeWall();
}
    
    int getWidth() const
    {
        return width_;
    }
    void setWidth(int width) {
        width_=width;
    }
    int getHeight() const
    {
        return height_;
    }
    void setHeight(int height){
        height_=height;
    }

    void initializeWall()
    {
       wall_ = std::vector<std::vector<Object *> >(height_, std::vector<Object *>(width_, nullptr));
    }
    int random_num()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 100);
        return dis(gen);
    }

 

    //Function crating new row at the top each move or during game creation
    void addNewRow(int score)
    {
        std::vector<Object *> line;
        std::srand(std::time(0));
        std::vector<sf::Color> vec_colors = vec_color();

        

        for (int i = 0; i < width_; i++)
        {
            int random_number = random_num();
            int player_score = score;
            float initialStartPoint_x;
            float initialStartPoint_y;
            // cheching if i is not a first ball
            sf::Color previousColor ;
            if (i > 0)
            {
                previousColor = line[i - 1]->getColor();
                
            }
            else
            {
                previousColor = vec_colors[(std::rand() % 3) + 1];
            }

            
            if (previousColor == vec_colors[0])
            {
                previousColor = vec_colors[1];
            }
            
            int randomDestruciton = (std::rand() % 6) + 1;
            //Special condition for different levels in game 50, 150, and third level for more than 150 points
            if (player_score < 50)
            {
                if (random_number <= 40)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i) * 45.f, initialStartPoint_y), previousColor, false);
                    line.push_back(ball);
                }
                else if (random_number <= 55)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i) * 45.f, initialStartPoint_y), vec_colors[1], false);
                    line.push_back(ball);
                }
                else if (random_number <= 70)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i) * 45.f, initialStartPoint_y), vec_colors[2], false);
                    line.push_back(ball);
                }
                else if (random_number <= 85)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i) * 45.f, initialStartPoint_y), vec_colors[3], false);
                    line.push_back(ball);
                }
                else
                {
                    Bomb *bomb = new Bomb(sf::Vector2f(20.f, 20.f), sf::Vector2f((i) * 45.f, initialStartPoint_y), vec_colors[0], true, randomDestruciton);

                    line.push_back(bomb);
                }
            }
            else if (player_score < 150 && player_score >=50)
            {
                if (random_number <= 35)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), previousColor, false);
                    line.push_back(ball);
                }
                else if (random_number <= 50)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[1], false);
                    line.push_back(ball);
                }
                else if (random_number <= 65)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[2], false);
                    line.push_back(ball);
                }
                else if (random_number <= 80)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[3], false);
                    line.push_back(ball);
                }
                else if (random_number <= 95)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[4], false);
                    line.push_back(ball);
                }
                else
                {
                    Bomb *bomb = new Bomb(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[0], true, randomDestruciton);
                    line.push_back(bomb);
                }
            }
            else
            {
                if (random_number <= 45)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), previousColor, false);
                    line.push_back(ball);
                }
                else if (random_number <= 55)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[1], false);
                    line.push_back(ball);
                }
                else if (random_number <= 65)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[2], false);
                    line.push_back(ball);
                }
                else if (random_number <= 75)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[3], false);
                    line.push_back(ball);
                }
                else if (random_number <= 85)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[4], false);
                    line.push_back(ball);
                }
                else if (random_number <= 95)
                {
                    Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[5], false);
                    line.push_back(ball);
                }
                else
                {
                    Bomb *bomb = new Bomb(sf::Vector2f(20.f, 20.f), sf::Vector2f((i + 1) * 45.f, initialStartPoint_y), vec_colors[0], true, randomDestruciton);
                    line.push_back(bomb);
                }
            }
        }
        for (int i = 0; i < width_; i++)
        {
            wall_[0][i] = line[i];
        }

    }
    //Function moves rows of balls a one row down
    void moveWall()
    {
        for (int i = height_ - 2; i >= 0; i--)
        {
            for (int j = 0; j < width_; j++)
            {
                if (wall_[i][j] != nullptr)
                {
                    std::swap(wall_[i][j], wall_[i + 1][j]);
                    if (wall_[i + 1][j] != nullptr)
                    {
                        sf::Vector2f new_position(j * 45.f, (i + 1) * 45.f);
                        wall_[i + 1][j]->setPosition(new_position);
                    }
                }
            }
        }
    }

};

class Game_menu
{
private:
    friend class Game;
    std::vector<Object* > menu_;
    int numOfBalls_;

public:
    Game_menu(int numOfBalls): numOfBalls_(numOfBalls){
        initializeMenu();
    }
    
    void initializeMenu()
    {
       menu_ = std::vector<Object *> (numOfBalls_);
    }

    int getNumOfBalls(){
        return numOfBalls_;
    }

    int random_ball(int a,int b)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(a, b);
        return dis(gen);
    }
    //Function adds new ball to queue
    void addNewBall(int score){
        std::vector<sf::Color> vec_colors = vec_color();
        if (score<50){
            Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f(45.f, 1600.f), vec_colors[random_ball(1,3)], false);
            menu_[0]=ball;
        }
        else if(score < 150 && score >=50){
            Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f(70.f, 1600.f), vec_colors[random_ball(1,4)], false);
            menu_[0]=ball;
        }
        else{
            Balls *ball = new Balls(sf::Vector2f(20.f, 20.f), sf::Vector2f(70.f, 1600.f), vec_colors[random_ball(1,5)], false);
                menu_[0]=ball;
        }
    }
    //Function moves balls one move forward.
    void  moveMenu(){
        for (int i = numOfBalls_- 2 ; i >= 0; i--)
        {
            if (i<numOfBalls_- 2){
                if (menu_[i]!= nullptr)
                {
                    std::swap(menu_[i], menu_[i + 1]);
                    if (menu_[i + 1] != nullptr)
                    {
                        sf::Vector2f new_position( (((i+1) * 90.f)+45.f), 1600.f);
                        menu_[i + 1]->setPosition(new_position);
                    }
                }
            }
            else if (i==(numOfBalls_- 2)){
                if (menu_[i]!= nullptr)
                {
                    std::swap(menu_[i], menu_[i + 1]);
                    if (menu_[i + 1] != nullptr)
                    {
                        sf::Vector2f new_position( 877.f, 1450.f);
                        menu_[i + 1]->setPosition(new_position);
                    }
                }

            }
        }
    }


};

    // Two remaining funciton for Game class needed to be here couse of variables inicialization for compiler

    //Start func to load all objects on window
    void Game::start(Game_wall &objects_wall, Game_menu &objects_menu, int score)
    {
        setScore(0);

        int ball_start_height = 14;

        //Makeing 2d vector with empty cells
        objects_wall.initializeWall();

        //loop adding row and moving it lower
        for (int i = 0; i < ball_start_height; i++)
        {
            objects_wall.addNewRow(score);
            objects_wall.moveWall();
            
        }
        objects_wall.addNewRow(score);
        
        // Making vector withempty cells for balls in queue
        objects_menu.initializeMenu();

        //loop adding ball and moving to shooting position and ther rest after
        for (int i = 1; i <= objects_menu.getNumOfBalls() ; i++){
            objects_menu.addNewBall(score);
            objects_menu.moveMenu();
        }
        objects_menu.addNewBall(score);
    }
    //Displaying whole game here
    void Game::dis(Game_wall &objects_wall, Game_menu &objects_menu, sf::RenderWindow &window)
    {
        //Game wall displayed here
        for (int i = 0; i < objects_wall.height_; i++)
        {
            for (int j = 0; j < objects_wall.width_; j++)
            {
                if (objects_wall.wall_[i][j] != nullptr)
                {
                
                window.draw(*(objects_wall.wall_[i][j]));
                
                }
            }
        }

        //Game menu dipsplayed here 
        for (int i = 0; i < objects_menu.getNumOfBalls(); i++)
        {
            if (objects_menu.menu_[i] != nullptr)
                {
                    window.draw(*(objects_menu.menu_[i]));
                }

        }
        
        
    }

    
   

int main()
{
    //Class objects inicialization
    Game_wall objects_wall(40,37);

    //Game_menu with inicialization with 8 balls in queue
    Game_menu objects_menu(8);
    //Game_menu with inicialization with score starting from 0
    Game game(0);
    //game loading
    
    sf::RenderWindow window(sf::VideoMode(1800, 1680), "Ball Shooter Game");
    game.start(objects_wall, objects_menu, game.getScore());

    // sf::RectangleShape redLine(sf::Vector2f(gameWall.width_ * 45.f, 20.f));
    // redLine.setFillColor(sf::Color::Red);
    // redLine.setPosition(0.f, 1300.f);

    // sf::RectangleShape blackLine(sf::Vector2f(gameWall.width_ * 45.f, 5.f));
    // blackLine.setFillColor(sf::Color::Black);
    // blackLine.setPosition(0.f, 1450.f);

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
               window.close();
        }       

        // Clear the window
        window.clear(sf::Color(240, 220, 180));

        //displaying whole game
        game.dis(objects_wall, objects_menu, window);

        // // Draw the red line
        // window.draw(redLine);

        // // Draw the black line
        // window.draw(blackLine);

        // Display the window
        window.display();
    }

    return 0;
}