CPPFLAGS= -I. -I/opt/opencv/include
CXXFLAGS= -Wall -Wextra -Werror
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
LDLIBS=\
	-lopencv_core\
	-lopencv_imgproc\
	-lopencv_highgui

#modify DATA or VIEWER path command if not applicable
VIEWER= pvisu
DATA= ../data

OBJ= morphology.o
BIN= \
	make-se\
	dilation\
	erosion\
	opening\
	closing\
	labeling-color\
	binarization\
	difference\
	add-boundary
	

.PHONY: all
all: $(BIN)

$(BIN): morphology.o

.PHONY: test-deploy
test-deploy: all
	@./make-se ; true
	@./dilation ; true
	@./erosion ; true
	make extract-gear
	make extract-cell

.PHONY: extract-gear
extract-gear: make-se opening difference binarization labeling-color
	./make-se 2 10 disk-10.png
	./opening disk-10.png $(DATA)/gear.png gear-erosion.png
	./difference gear-erosion.png $(DATA)/gear.png gear-res.png
	$(VIEWER) gear-res.png
	./binarization 230 255 gear-res.png gear-binarization.png
	./labeling-color gear-binarization.png gear-reg.png gear-res-color.png
	$(VIEWER) gear-res-color.png

.PHONY: extract-cell
extract-cell: make-se opening binarization dilation difference add-boundary
	./make-se 2 9 disk-9.png
	./opening disk-9.png $(DATA)/cell.png cell-opening.png
	./binarization 175 255 cell-opening.png cell-res.png
	$(VIEWER) cell-res.png
	./labeling-color cell-res.png cell-reg.png cell-res-color.png 
	$(VIEWER) cell-res-color.png
	
	./make-se 2 5 disk-5.png
	./dilation disk-5.png cell-res.png cell-dilation.png
	./difference  cell-res.png cell-dilation.png cell-res-boundary.png
	./add-boundary $(DATA)/cell.png cell-res-boundary.png cell-final.png
	$(VIEWER) cell-res-boundary.png
	$(VIEWER) cell-final.png
	
	
	
.PHONY: clean
clean:
	$(RM) *~ *.png

.PHONY: cleanall
cleanall: clean
	$(RM) $(BIN) *.o *.pdf
