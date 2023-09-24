# Get the fully qualified path to the script
case $0 in
    /*)
        SCRIPT="$0"
        ;;
    *)
        PWD=`pwd`
        SCRIPT="$PWD/$0"
        ;;
esac

# Change spaces to ":" so the tokens can be parsed.
SCRIPT=`echo $SCRIPT | sed -e 's; ;:;g'`
# Get the real path to this script, resolving any symbolic links
TOKENS=`echo $SCRIPT | sed -e 's;/; ;g'`
REALPATH=
for C in $TOKENS; do
    REALPATH="$REALPATH/$C"
    while [ -h "$REALPATH" ] ; do
        LS="`ls -ld "$REALPATH"`"
        LINK="`expr "$LS" : '.*-> \(.*\)$'`"
        if expr "$LINK" : '/.*' > /dev/null; then
            REALPATH="$LINK"
        else
            REALPATH="`dirname "$REALPATH"`""/$LINK"
        fi
    done
done
# Change ":" chars back to spaces.
REALPATH=`echo $REALPATH | sed -e 's;:; ;g'`

curPath="`dirname "$REALPATH"`"

# Change the current directory to the location of the script
cd "$curPath"

# echo $curPath

if [ ! -d "db" ]
then
	mkdir db
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$curPath

# echo $LD_LIBRARY_PATH

./icebox --Ice.Config=icebox.config --daemon --nochdir