#! /bin/sh

if [ $SHELL == "/bin/sh" ]
then
  `which bash` $0
  exit 0;
fi

if [ -e config.lastrun ]
then
	source config.lastrun
fi

touch config.lastrun~
touch config.capab~

LIBS="-lm -lc"
MODULES=""
LOCATION=`pwd`

echo "When prompted, please enter your choice from the square brackets.";
echo "Your previous answers are displayed in parenthese.";
echo "Hitting enter will cause it to use your previous answer.";
echo "";
echo "";
echo "Detecting OS requirements...";
echo "============================";
echo "";
echo -n "Looking for dl... ";
if [ -e /usr/lib/libdl.a ]
then
	echo "found, using -ldl"
	DL=1
	LIBS="$LIBS -ldl"
else
	DL=0
	echo "not found"
fi

echo "";
echo "Attempting to detect OS capabilities";
echo "====================================";
echo "";

#######################################################################
#######################################################################
#######################################################################

echo ""
echo "Creating Makefiles...";

#####################################

echo ""
echo "includes/Make_config";

cd includes/;

echo "CC = gcc" > Make_config;
echo "CFLAGS = -export-dynamic -g -fPIC -pipe -I\$(INCLUDE_DIR)" >> Make_config;
echo "LIBS = $LIBS" >> Make_config;
echo -n "INCLUDES =" >> Make_config;
for X in `grep -L "CHAINED: IGNORE" * | grep \.h`
do
	echo " \$(INCLUDE_DIR)$X \\" >> Make_config;
done;

cd ..

####################################

echo "corelib/Makefile";
cd corelib

OBJS=`grep -LE "CHAINED: (IGNORE|NORULE)" * | grep \\\.c | sed s/\\\.c\$//g`
echo "INCLUDE_DIR = ../includes/" > Makefile
echo "" >> Makefile
echo "include \$(INCLUDE_DIR)Make_config" >> Makefile
echo "" >> Makefile
echo -n "OBJS =" >> Makefile
for X in $OBJS
do
	echo -n " $X.o" >> Makefile
done;
echo "" >> Makefile
echo "" >> Makefile
echo "all:	\$(OBJS)" >> Makefile
echo "	\$(CC) \$(CFLAGS) \$(LIBS) -shared -o libchained.so \$(OBJS)" >> Makefile
echo "" >> Makefile
echo "# ----" >> Makefile
echo "" >> Makefile
echo "\$(OBJS):	\$(INCLUDES)" >> Makefile
echo "" >> Makefile
echo "# ----" >> Makefile
echo "" >> Makefile
echo "clean:" >> Makefile
echo "	rm -f *.o *.so" >> Makefile

cd ..
#####################################

echo "Makefile";

echo "all:" > Makefile
echo "	(cd corelib; make all)" >> Makefile
echo "	(cd echo; make all)" >> Makefile
echo "" >> Makefile
echo "clean:" >> Makefile
echo "	(cd corelib; make clean)" >> Makefile
echo "	(cd echo; make clean)" >> Makefile
echo "" >> Makefile


mv config.lastrun~ config.lastrun;
echo "";
echo "------------------------------";
echo "      Config complete.";
echo "------------------------------";
echo "";
