#include "qtconceptmapmode_test.h"

#include "qtconceptmapmode.h"


#include <QDebug>

void ribi::cmap::ModeTest::str_conversion() const noexcept
{
  const auto v = collect_all_modes();
  for (const auto m: v)
  {
    const std::string s = to_str(m);
    const Mode m_again = str_to_mode(s);
    QVERIFY(m == m_again);
  }
}
