/**
 * @file    utils.hpp
 * @author virtual_enaction_C@enseirb-matmeca.fr
 *
 * @brief  Décrit les structures utilisées dans botplug.
 *
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <math.h>

namespace botplug {
const double BLOCK_SIZE = 1;

/**
 * @brief FR : Représente une Position par un triplet de flottant.
 * EN : Represent a Position by 3 float
 */
class Position {
public:
  /**
   * @brief FR : composante sur x / EN : x component
   */
  float _x;

  /**
   * @brief FR : composante sur y / EN : y component
   */
  float _y;

  /**
   * @brief FR : composante sur z / EN : z component
   */
  float _z;

  /**
   * @brief FR : Constructeur par défaut / EN : Default constructor
   */
  Position() : _x(0), _y(0), _z(0) {}

  /**
   * @brief FR : Constructeur prenant les 3 composantes de la Position
   * EN : Constructor taking in argument the 3 component of Position
   * @param x
   * @param y
   * @param z
   */
  Position(float x, float y, float z) : _x(x), _y(y), _z(z) {}

  /**
   * @brief FR : Constructeur par copie
   * EN : Copy constructor
   * @param p
   */
  Position(const Position &p) : _x(p._x), _y(p._y), _z(p._z) {}

  void normalize()
  {
    double norm = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
    _x = _x / norm;
    _y = _y / norm;
    _z = _z / norm;
  }
  void rotate(const double yaw, const double pitch, const double roll)
  {
    // Standard rotation matrix for real axes.
    _x = _x * cos(yaw) * cos(pitch) +
         _y * sin(yaw) * cos(pitch) -
         _z * sin(pitch);
    _y = _x * (cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll)) +
         _y * (sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll)) +
         _z * cos(pitch) * sin(roll);
    _z = _x * (cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll)) +
         _y * (sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll)) +
         _z * cos(pitch) * cos(roll);
  }
  void MCrotate(const double yaw, const double pitch)
  {
    // Rotation matrix for Minecraft environment
    double xo = _x;
    double yo = _y;
    double zo = _z;

    // Rotation along X-axis (up-down)

    _y = yo * cos(pitch) + sin(pitch) * zo;
    _z = -yo *sin(pitch) + cos(pitch) * zo;

    // Rotation along Y-axis (left-right)
    _x = xo * cos(yaw) - _z * sin(yaw);
    _z = xo * sin(yaw) + _z * cos(yaw);

    /*_y = yo * cos(pitch) + sin(pitch) * _z;
     *   _z = -yo* sin(pitch) + cos(pitch) * _z;*/
  }
  static double distance(const Position& p1, const Position& p2)
  {
    return sqrt(pow(p1._x - p2._x, 2) + pow(p1._y - p2._y, 2) + pow(p1._z - p2._z, 2));
  }
};

struct direction {
  /**
   * @brief FR : le yaw : angle horizontale
   * EN : yaw is the horizontal angle
   */
  float _yaw;

  /**
   * @brief FR : le pitch : composante verticale
   * pitch : vertical component
   */
  float _pitch;

  /**
   * @brief FR : Constructeur par défaut / EN : Default constructor
   */
  direction() : _yaw(0), _pitch(0) {}

  /**
   * @brief FR : Constructeur prenant les 3 composantes de la Position
   * EN : Constructor with the 3 component of the Position in argument
   * @param yaw
   * @param pitch
   */
  direction(float yaw, float pitch) : _yaw(yaw), _pitch(pitch) {}

  /**
   * @brief FR : Constructeur par copie
   * EN : Copy constructor
   * @param p
   */
  direction(const direction &p) : _yaw(p._yaw), _pitch(p._pitch) {}
};

/**
 * @brief FR : Représente une entité Minecraft.
 * EN : Represent a Minecraft entity
 *
 * FR : Représente une entité Minecraft avec un identifiant unique et une
 * botplug::Position.
 * EN : Represent a Minecraft entity with an unique identifier and
 * a botplug::Position.
 */
struct Entity {
  /**
   * @brief FR : identifiant unique de l'entité Minecraft
   * EN : unique identifier of the Minecraft entity
   * See: The <a href="http://minecraft.gamepedia.com/Data_values#Entity_IDs">entity IDs</a>
   */
  int _id;
  int _typeId;
  /**
   * @brief FR : Position de l'entité sur la map Minecraft
   * EN : The Minecraft entity's Position on the map
   */
  Position _p;

  /**
   * @brief Constructeur par défaut
   */
  Entity() : _id(0), _p() {}

  /**
   * @brief FR : Constructeur avec paramètres
   * EN : Constructor with parameters
   * @param id
   * @param x
   * @param y
   * @param z
   */
  Entity(int id, float x, float y, float z) : _id(id), _p(x, y, z) {}

  /**
   * @brief FR : Constructeur par copie
   * EN : Copy constructor
   * @param e
   */
  Entity(const Entity &e) : _id(e._id), _p(e._p) {}
};

/**
 * @brief FR : Représente un Block Minecraft.
 * EN : Represent a Minecraft Block
 *
 * FR : Représente un Block Minecraft avec un identifiant unique, une botplug::Position, une
 * température et des coefficients de rugosité et dureté.
 * EN : Represent a Minecraft Block with an unique identifier, a botplug::Position, a
 * temperature and coefficients of roughness and hardness.
 *
 */
struct Block {
  /**
   * @brief FR : identifiant unique du Block Minecraft
   * EN : unique identifier of the Minecraft Block
   * See: The <a href="http://www.minecraftinfo.com/idlist.htm">ID list</a>.
   */
  int _id;

  /**
   * @brief FR : Position du Block sur la map Minecraft
   * EN : Position of the Minecraft Block on the map
   */
  Position _p;

  /**
   * @brief FR : coefficient de rugosité
   * EN : roughness coefficient
   */
  float _roughness;

  /**
   * @brief FR : coefficient de dureté
   * EN : hardness coefficient
   */
  float _hardness;

  /**
   * @brief FR : température du Block
   * EN : Block's temperature
   */
  float _temperature;

  /**
   * @brief FR : couleur du bloc
   * EN : color of the Block
   */
  int _color;

  /**
   * @brief FR : distance entre le bloc et le bot
   * EN : Distance between the Block and the bot
   */
  double _distance;

  /**
   * @brief FR : Constructeur par défaut
   * EN : Default constructor
   */
  Block() :
    _id(0), _p(), _roughness(0), _hardness(0), _temperature(0), _color(0), _distance(0.0)
  {}

  /**
   * @brief FR : Constructeur par id
   * EN : Constructor by id
   */
  Block(int id) :
    _id(id), _p(), _roughness(0), _hardness(0), _temperature(0), _color(0), _distance(0.0)
  {}

  /**
   * @brief FR : Constructeur avec paramètres
   * EN : Constructor with parameters
   * @param id
   * @param x
   * @param y
   * @param z
   * @param roughness
   * @param hardness
   * @param t
   */
  Block(int id,
        float x, float y, float z,
        float roughness, float hardness,
        float t) :
    _id(id), _p(x, y, z), _roughness(roughness), _hardness(hardness), _temperature(t), _color(getColorFromId(id)), _distance(0.0)
  {}

  /**
   * @brief FR : Constructeur avec paramètres
   * EN : Constructor with parameters
   * @param id
   * @param p
   * @param roughness
   * @param hardness
   * @param t
   */
  Block(int id,
        Position p,
        float roughness, float hardness,
        float t) :
    _id(id), _p(p), _roughness(roughness), _hardness(hardness), _temperature(t), _color(getColorFromId(id)), _distance(0.0)
  {}

  /**
   * @brief FR : Constructeur par copie
   * EN : Copy constructor
   * @param blk
   */
  Block(const Block &blk) :
    _id(blk._id), _p(blk._p), _roughness(blk._roughness), _hardness(blk._hardness), _temperature(blk._temperature), _color(blk._color), _distance(blk._distance)
  {}

  // TODO : faire en sorte que cette fonction retourne une couleur "globale" pour cet objet
  static int getColorFromId(int id)
  {
    return id;
  }
  double getDistanceFrom(const botplug::Position& pos) const
  {
    Position dest = _p;
    dest._x += botplug::BLOCK_SIZE / 2;
    dest._y += botplug::BLOCK_SIZE / 2;
    dest._z += botplug::BLOCK_SIZE / 2;

    return Position::distance(pos, dest);
  }
};

/**
 * @brief FR : Représente un item Minecraft.
 * EN : Represent a Minecraft item
 *
 * FR : Représente un item Minecraft grâce à un identifiant unique,
 * un emplacement d'inventaire et une quantité.
 * EN : Represent a Minecraft item with an unique identifier,
 * a place in the inventory and a quantity
 */
struct item {
  /**
   * @brief FR : l'identifiant unique de l'item
   * EN : Unique identifier of the item
   * See: The <a href="http://www.minecraftinfo.com/idlist.htm">ID list</a>.
   */
  int _id;

  /**
   * @brief FR : l'emplacement de l'item dans l'inventaire du joueur Minecraft
   * EN : The position of the item in the Minecraft player inventory
   */
  int _slot;

  /**
   * @brief FR : la quantité de _Block à l'emplacement _slot
   * EN : The quantity of _Block at the place _slot
   */
  int _number;

  /**
   * @brief FR : Constructeur par défaut / EN : Default constructor
   */
  item() : _id(-1), _slot(-1), _number(0) {}

  /**
   * @brief FR : Constructeur avec paramètres avec Block par défaut
   * EN : Constructor with parameters with default Block
   * @param slot
   * @param number
   */
  item(int slot, int number) : _id(-1), _slot(slot), _number(number) {}

  /**
   * @brief FR : Constructeur avec paramètres
   * EN : Constructor with parameters
   * @param id
   * @param slot
   * @param number
   */
  item(int id, int slot, int number) : _id(id), _slot(slot), _number(number) {}

  /**
   * @brief FR : Teste si l'item est vide
   * EN : Test if the item is empty
   * @return FR : true si l'item est vide, false sinon
   * EN : true is the item is empty, else false
   */
  bool empty()
  {
    return _slot == -1;
  }
};
}

#endif
