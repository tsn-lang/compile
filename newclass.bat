@echo off

set /p class_name="Enter class name: "
set /p dir_name="Enter dir name: "

set header_name="%class_name%.h"
set src_name="%class_name%.cpp"

set printf="C:\Program Files\Git\usr\bin\printf.exe"

cd "./include/compile/%dir_name%"
%printf% "#pragma once\n" >> %header_name%
%printf% "#include <compile/types.h>\n\n" >> %header_name%
%printf% "namespace compile {\n" >> %header_name%
%printf% "    class %class_name% {\n" >> %header_name%
%printf% "        public:\n" >> %header_name%
%printf% "            %class_name%();\n" >> %header_name%
%printf% "            ~%class_name%();\n" >> %header_name%
%printf% "    };\n" >> %header_name%
%printf% "};" >> %header_name%

IF "%dir_name%" == "" (
    cd "../../src/%dir_name%"
    %printf% "#include <compile/%header_name%>\n\n" >> %src_name%
) ELSE (
    cd "../../../src/%dir_name%"
    %printf% "#include <compile/%dir_name%/%header_name%>\n\n" >> %src_name%
)
%printf% "namespace compile {\n" >> %src_name%
%printf% "    %class_name%::%class_name%() {\n" >> %src_name%
%printf% "    }\n" >> %src_name%
%printf% "\n" >> %src_name%
%printf% "    %class_name%::~%class_name%() {\n" >> %src_name%
%printf% "    }\n" >> %src_name%
%printf% "};" >> %src_name%

IF "%dir_name%" == "" (
    cd "../"
) ELSE (
    cd "../../"
)
copy /b CMakeLists.txt +,,