- Plan ahead right after kickoff
  - Plan the auto routines, select drivers, designate work early. 
  - Plan the driver interfaces, and how much will be controlled by human vs robot
  - Plan the software architecture right away, and structure your experiments to fit into it
- Make field elements
- Use the old robot and other test platforms to test the software
- Do it in cad first
- Design the robot/software for the drive team
- Make a test base (can be an old robot) and keep a git branch with the code for it
  - Keep every essential part on both robots (ex: pi, pigeon, same drivetrain) but small variations don't matter (ex: different motor, different size, different cylinders)
  - Remember that this robot doesn't need to pass inspection 
- Make sure to collaborate more, on the software and hardware sides. 
  - Have the people working on cad collaborate to figure out who will be working on what, and modifying files. 
  - Have Programming talk about how different systems fit together, and once it is decided, document it. 
- Use doxygen, as well as flowcharts that show how things fit together. 
- Do branch protections better
  - Only let people who are involved with the code that is being modified approve it
  - Actually review the pull requests instead of clicking approve and moving on
  - Don't treat another branch like main; just use main
  - potential idea: Have "file owners"; people who must approve any PR with changes to those files


## off season
- Organize bins, and store them better
- Make better wrench storage
