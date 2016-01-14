#!/bin/bash
rm -rf $@.app
mkdir -p $@.app
mkdir -p $@.app/lib
mkdir -p $@.app/MacOS
mkdir -p $@.app/Recources
cp $@ $@.app/MacOS/
cp font.png $@.app/MacOS/
cp -r audio $@.app/MacOS/
cp icon.icns $@.app/Recources/
chmod a+x $@.app/MacOS/$@
otool -L $@ | awk '{if(NR>1)print}' | while read -r line ; do
    file=${line%%'('*}
    cp $file ./$@.app/lib/
    install_name_tool -change $file ../lib/$file $@.app/MacOS/$@
done

text="
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
  <key>CFBundleName</key>
  <string>$@</string>
  <key>CFBundleIconFile</key>
  <string>icon.icns</string>
  <key>CFBundleExecutable</key>
  <string>$@</string>
</dict>
</plist>
"


printf "$text" > $@.app/Info.plist