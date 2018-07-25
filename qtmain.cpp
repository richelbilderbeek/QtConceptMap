#include "qtconceptmap.h"

#include <iostream>

#include <string>
#include <vector>

#include <QApplication>
#include <QDesktopWidget>

#include "qtconceptmapcommands.h"
#ifdef SOME_EXAMPLES_TO_BE_USED
--command "set_mode(edit); create_new_node(center, true, 0, 0); unselect(center); create_new_node(from, false, 0, -50); create_new_node(to, false, 0, 50); create_new_edge(relation); move(relation, 200, 0); toggle_arrow_head(); toggle_arrow_tail()" //!OCLINT
--command "set_mode(edit); create_new_node(center, true, 0, 0); unselect(center); create_new_node(from, false, 0, -50); create_new_node(to, false, 0, 50); create_new_edge(relation); move(relation, 200, 0); toggle_arrow_head(); toggle_arrow_tail()" //!OCLINT
#endif

std::vector<std::string> get_args(const int argc, char ** argv)
{
  std::vector<std::string> v(argc);
  for (int i=0; i!=argc; ++i) v[i] = argv[i];
  return v;
}

int get_screen_height(
  const std::vector<std::string>& args,
  const int default_height = 768)
{
  const int sz = args.size();
  for (int i=0; i!=sz-1; ++i)
  {
    if (args[i] == "--height")
    {
      try
      {
        return std::stoi(args[i + 1]);
      }
      catch(std::exception&) {}; //!OCLINT
    }
  }
  return default_height;
}

int get_screen_width(
  const std::vector<std::string>& args,
  const int default_width = 1024)
{
  const int sz = args.size();
  for (int i=0; i!=sz-1; ++i)
  {
    if (args[i] == "--width")
    {
      try
      {
        return std::stoi(args[i + 1]);
      }
      catch(std::exception&) {}; //!OCLINT
    }
  }
  return default_width;
}

bool has_help(const std::vector<std::string>& args)
{
  return std::count(std::begin(args), std::end(args), std::string("--help"));
}

void show_help()
{
  std::cout
    << "QtConceptMapDemo" << '\n'
    << "----------------" << '\n'
    << "" << '\n'
    << "Demonstrates the QtConceptMap class" << '\n'
    << "" << '\n'
    << "--help: show help" << '\n'
    << "--width 640: set width of screen to 640" << '\n'
    << "--height 400: set width of screen to 400" << '\n'
    << "--command \"create_new_node(10, 20, my text)\"" << '\n'
  ;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  const std::vector<std::string> args = get_args(argc, argv);
  if (has_help(args))
  {
    show_help();
    return 0;
  }

  ribi::cmap::QtConceptMap d;
  d.SetMode(ribi::cmap::Mode::edit);
  d.show();
  d.setGeometry(0, 0, get_screen_width(args), get_screen_height(args));
  //Put the dialog in the screen center
  const QRect screen = QApplication::desktop()->screenGeometry();
  d.move(screen.center() - d.rect().center() );

  ribi::cmap::ProcessCommands(d, args);
  return a.exec();
}
