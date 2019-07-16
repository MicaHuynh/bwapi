#include <BWAPI/Position.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Region.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Player.h>

#include <BWAPI/UnitSizeType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/ExplosionType.h>
#include <BWAPI/WeaponType.h>

#include <cstdarg>

// Needed by other compilers.
#include <cstring>

#include "GameImpl.h"

namespace BWAPI
{
  //------------------------------------ DAMAGE CALCULATION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI::Position p)
  {
    this->setScreenPosition(p.x, p.y);
  }
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->pingMinimap(p.x, p.y);
  }
  void GameImpl::sendText(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(false, format, ap);
    va_end(ap);
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vSendTextEx(toAllies, format, ap);
    va_end(ap);
  };
  void GameImpl::printf(const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vPrintf(format, ap);
    va_end(ap);
  };
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(int tileX, int tileY, UnitType unitType) const
  {
    if ( unitType.isValid() && unitType < UnitTypes::None )
      return hasPowerPrecise( tileX*32 + unitType.tileWidth()*16, tileY*32 + unitType.tileHeight()*16, unitType);
    return hasPowerPrecise( tileX*32, tileY*32, UnitTypes::None);
  }
  bool GameImpl::hasPower(TilePosition position, UnitType unitType) const
  {
    return hasPower(position.x, position.y, unitType);
  }
  bool GameImpl::hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPowerPrecise( tileX*32 + tileWidth*16, tileY*32 + tileHeight*16, unitType);
  }
  bool GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType) const
  {
    return hasPower(position.x, position.y, tileWidth, tileHeight, unitType);
  }
  bool GameImpl::hasPowerPrecise(Position position, UnitType unitType) const
  {
    return hasPowerPrecise(position.x, position.y, unitType);
  }
  //------------------------------------------ MAP DATA -----------------------------------------------
  bool GameImpl::isWalkable(BWAPI::WalkPosition position) const
  {
    return this->isWalkable(position.x, position.y);
  }
  int GameImpl::getGroundHeight(TilePosition position) const
  {
    return this->getGroundHeight(position.x, position.y);
  }
  bool GameImpl::isBuildable(TilePosition position, bool includeBuildings) const
  {
    return this->isBuildable(position.x, position.y, includeBuildings);
  }
  bool GameImpl::isVisible(TilePosition position) const
  {
    return this->isVisible(position.x, position.y);
  }
  bool GameImpl::isExplored(TilePosition position) const
  {
    return this->isExplored(position.x, position.y);
  }
  bool GameImpl::hasCreep(TilePosition position) const
  {
    return this->hasCreep(position.x, position.y);
  }
  Unitset GameImpl::getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred) const
  {
    return this->getUnitsOnTile(TilePosition(tileX,tileY), pred);
  }
  Unitset GameImpl::getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred) const
  {
    if ( !tile )  // if tileposition not valid
      return Unitset::none;

    Position p(tile); // convert to pixel position
    return this->getUnitsInRectangle(p.x, p.y, p.x + 32, p.y + 32, pred);
  }
  Unitset GameImpl::getUnitsInRadius(int x, int y, int radius, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(x - radius,
                                     y - radius,
                                     x + radius,
                                     y + radius,
                                     [&x,&y,&radius,&pred](Unit u){ return u->getDistance(Position(x,y)) <= radius && (!pred.isValid() || pred(u)); });
  }
  Unitset GameImpl::getUnitsInRadius(Position center, int radius, const UnitFilter &pred) const
  {
    return this->getUnitsInRadius(center.x, center.y, radius, pred);
  }
  Unitset GameImpl::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred) const
  {
    return this->getUnitsInRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, pred);
  }
  Unit GameImpl::getClosestUnit(Position center, const UnitFilter &pred, int radius) const
  {
    return this->getClosestUnitInRectangle(center,
                                            [&](Unit u){ return u->getDistance(center) <= radius && (!pred.isValid() || pred(u));},
                                            center.x - radius,
                                            center.y - radius,
                                            center.x + radius,
                                            center.y + radius);
  }
  //------------------------------------------ REGIONS -----------------------------------------------
  BWAPI::Region GameImpl::getRegionAt(BWAPI::Position position) const
  {
    return this->getRegionAt(position.x, position.y);
  }
  bool GameImpl::hasPath(Position source, Position destination) const
  {
    if (source.isValid() && destination.isValid())
    {
      Region rgnA = getRegionAt(source);
      Region rgnB = getRegionAt(destination);
      if (rgnA && rgnB && rgnA->getRegionGroupID() == rgnB->getRegionGroupID())
        return this->setLastError();
    }
    return this->setLastError(Errors::Unreachable_Location);
  }
  //------------------------------------------ DRAW TEXT ----------------------------------------------
  void GameImpl::drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...)
  {
    va_list ap;
    va_start(ap,format);
    this->vDrawText(ctype, x, y, format, ap);
    va_end(ap);
  }
}

