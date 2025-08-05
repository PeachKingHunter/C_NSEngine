Welcome on my repo where i'm creating an 'game engine' for create an game on it.

The game is an UI based game and incremental.

The project is now "finished".

Doc: (done after possibly not exact, you can look at gameFolder)
- Operands in .Info:
  - objType: Window/Button/TextLabel
  - textSize: <number> (10 is default)
  - textColor: <number> <number> <number> <number> (rgb and opacity, min 1 arg)
  - backgroundColor: <number> <number> <number> <number> (rgb and opacity, min 1 arg)
  - borderColor: <number> <number> <number> <number> (rgb and opacity, min 1 arg)
  - hide (just hide the UI_component because it's show by default)
  - show (useless in .Info)
  - pos: <number> <number>
  - onClick: <scriptName>
  - text: <text> (can be put in "")
    - ex: text: "text vraiment = 17"
    - because text: text vraiment = 17 will write "text vraiment 17" due to that = is used for special things.
    - Also if a word is an variable name it is automatically changed (other utility of "" for make bigger word container variable name without pb)
  - size: <number> <number>


- Categories of script:
  - .Start -> Before load variables (where loadOrCreate should be put (see it after))
  - .StartLate -> after loading saved variables
  - .Script -> can be call by buttons or other scripts
  - .Update -> each frame


- Operands in .Script
  - if <condition> {
  
  }
  - if <condition> then
  
  end
  - printf
  - changeTextOf(<objectName>, <text>);
  - changeImageOf(<objectName>, <imageName>); -> image should be in images/
  - changeBackgroundColorOf(<objectName>, <imageName>);
  - changeTextColorOf(<objectName>, <number>, <number>, <number>, <number>);
  - startScript(<scriptName>, <varNameInNextScript>, <valueOfTheVar>...); -> only scriptName is not facultative, no limit of var for the called script
  - int <varName> = <value>; -> access only in the script
  - intG <varName> = <value>; -> acces by any script
  - loadOrCreate <varName> = <value>; -> save as intG but saved and load after .Start
  - setSizeOf(<objectName>, <number>, <number>);
  - hideOf(<objectName>);
  - showOf(<objectName>);
  - add <varIntoAdding> <var/number> -> add the second to varIntoAdding
  - minus <var> <var/number>
  - mult <var> <var/number>
div <var> <var/number>
randInto(<var>, <min>, <max>);

This project use SDL3, SDL3_image and SDL3_ttf as graphics lib.
