CPPFLAGS= -I. -I/opt/opencv/include
CXXFLAGS= -Wall -Wextra -Werror
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
LDLIBS=\
	-lopencv_core\
	-lopencv_imgproc\
	-lopencv_highgui\
	-lopencv_features2d\
	-lopencv_nonfree\
	-lopencv_calib3d
	

#modify DATA or VIEWER path command if not applicable
VIEWER= pvisu
DATA= ../data

BIN= \
	feature-detection
	

.PHONY: all
all: $(BIN)

	
.PHONY: clean
clean:
	$(RM) *~ *.png

.PHONY: cleanall
cleanall: clean
	$(RM) $(BIN) *.o *.pdf
