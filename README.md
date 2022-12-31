# wallplotter
Software for a plotting device for big areas. 
Feature Branche naming convention: <initials>_<plattform>-<feature>
Example: js_esp32-test
Workflow:
  1. Individual developement in feature branches
  2. Merge in dev
  3. When code is compiling in dev merge into branche quality assurance(qa) 
  4. Code in branche qa is tested on the robot
  5. When is working it is merged into main
  6. When it's not working changes need to be made in the feature branches and the workflow repeats itself
