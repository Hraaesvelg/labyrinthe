
#This is a template to build your own project with the e-puck2_main-processor folder as a library.
#Simply adapt the lines below to be able to compile

# Define project name here
PROJECT = Labyrinthe

#Define path to the e-puck2_main-processor folder
GLOBAL_PATH = C:\Users\Valentin\Desktop\Microinformatique-TP\autres\e-puck2_main-processor
#../e-puck2_main-processor

#Source files to include
CSRC += ./main.c \
		./usageMotors.c\
		./actionUser.c\
		./allThreads.c\
		./color.c\
		
#Header folders to include
INCDIR += 

#Jump to the main Makefile
include $(GLOBAL_PATH)/Makefile
