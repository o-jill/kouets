# Project file specification

1. plain text format
 * specify files to be processed.
 * application, command line and parser in your settings are used.
 * a line which starts with '#' is skipped as comment.
 * example

```ruby
# kouets plaintext format
#
kouetsapp.cpp
kouetsapp.h
main.cpp
mainwindow.cpp
mainwindow.h
projectfile.cpp
projectfile.h
decorate.cpp
decorate.h
```

2. XML format
 * tag name are not case sensitive. e.g. <"KOUETS"> and <"kouets"> are same.
 * to specify < and >, the sentence should be double-quoted.
 * example

```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<kouets version="1.0">
 <config>
  <apppath> C:/path/to/default.exe </apppath> <!-- specify default value -->
  <cmdline> --some --default --command --option </cmdline> <!-- specify default value -->
  <!-- activatetab> FALSE </activatetab -->
  <!-- wrapline> TRUE </wrapline -->
  <!-- color1> #FF0000 </color1 -->
 </config>
 <item>
  <file>kouetsapp.cpp</file>
  <!-- default program(apppath) and cmdline  will be used -->
 </item>
 <item>
  <file> kouetsapp.h </file>
  <apppath> C:/speci/alpat/htoan/application.exe </apppath> <!-- only for kouetsapp.h -->
  <cmdline> --some --command --option </cmdline> <!-- only for kouetsapp.h -->
  <parser> cpplint.py_VS7 </parser>
 </item>
</kouets>
```

3. supported parsers

|   typename   | supports what?                             |
|:------------:|:-------------------------------------------|
|cpplint.py_VS7|google c++ lint program with "--output=vs7".|

----
