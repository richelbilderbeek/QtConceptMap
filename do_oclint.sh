#!/bin/bash

cpp_files=`find . | egrep ".*\.cpp$" | egrep -v "^qrc_.*\.cpp$" | egrep -v "^moc_.*\.cpp$" | egrep -v "^.*_test\.cpp$" | egrep "^./CppQtConce"`
h_files=  `find . | egrep ".*\.h$"   | egrep -v "^ui_.*\.h$"    | egrep "^./CppQtConce"`

./oclint-0.10.3/bin/oclint -o oclint.log \
  -disable-rule ShortVariableName \
  $cpp_files \
  $h_files \
  -- \
  -c -std=c++14 -fPIC \
  -I./CppConceptMap \
  -I./CppContainer \
  -I./CppCounter \
  -I./CppDnaSequence \
  -I./CppFastaFile \
  -I./CppFileIo \
  -I./CppMusic \
  -I./CppNewick \
  -I./CppPolarCoordinat \
  -I./CppQtHideAndShowDialog \
  -I./CppQtKeyboardFriendlyGraphicsView \
  -I./CppRibiRinside \
  -I./CppRibiRegex \
  -I./CppTestTimer \
  -I./CppTrace \
  -I./CppUnits \
  -I./CppXml \
  -I../BoostGraphTutorial/BoostGraphTutorial \
  -I../RibiLibraries/bigint-2010.04.30 \
  -I../RibiLibraries/rinside \
  -I/usr/include/c++/5 \
  -I/usr/include/qt5 \
  -I/usr/include/qt5/QtCore \
  -I/usr/include/qt5/QtGui \
  -I/usr/include/qt5/QtWidgets

cat oclint.log

# Will be 0 if success
# Will be 1 if fail
fail=`egrep "Compiler Errors" oclint.log | wc -l`

if [ $fail -eq 1 ]; 
then
  echo "OCLint: Compiler error"
  exit 1
else
  echo "OCLint: OK"
fi

# Will be 1 if success
# Will be 0 if fail
success=`egrep "FilesWithViolations=0 P1=0 P2=0 P3=0" oclint.log | wc -l`

if [ $success -eq 1 ]; 
then
  echo "OCLint: OK"
else
  echo "OCLint: Fail"
  exit 1
fi