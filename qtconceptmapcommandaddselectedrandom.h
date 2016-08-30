//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H
#define CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapcommand.h"

namespace ribi {
namespace cmap {

///Add another vertex to being selected
///If one out of four nodes is selected, select a random second
class CommandAddSelectedRandom final : public Command
{
public:

  CommandAddSelectedRandom(ConceptMap& conceptmap);
  CommandAddSelectedRandom(const CommandAddSelectedRandom&) = delete;
  CommandAddSelectedRandom& operator=(const CommandAddSelectedRandom&) = delete;
  ~CommandAddSelectedRandom() noexcept {}

  void undo() override;
  void redo() override;

  private:
  ConceptMap& m_conceptmap;
  const ConceptMap m_before;
  const ConceptMap m_after;
};

//Throws if there are no nodes that can be selected additionally
ConceptMap RandomlyAddSelectedNode(ConceptMap g);

} //~namespace cmap
} //~namespace ribi


#endif // CONCEPTMAPCOMMANDADDSELECTEDRANDOM_H
