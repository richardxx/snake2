INSTALL_DIR=/usr/local/GreedySnake
AUDIO_DIR=/usr/local/GreedySnake/audio
SHADOW_DIR=/usr/bin
LINKAGE=-lncurses

snake:lovely_snake.o food.o system.o snake.o snake_list.o compet.o ani.o common_ds.o sound.o
	g++ lovely_snake.o food.o system.o snake.o snake_list.o compet.o ani.o common_ds.o sound.o ${LINKAGE} -O1 -o snake

snake_list.o:snake_list.cxx snake_list.h common_ds.h
	g++ -c -O1 snake_list.cxx 
	
lovely_snake.o:lovely_snake.cxx snake_list.h lovely_snake.h const_dec.h common_ds.h
	g++ -c -O1 lovely_snake.cxx
	
food.o:food.cxx common_ds.h const_dec.h food.h 
	g++ -c -O1 food.cxx

system.o:system.cxx system.h const_dec.h lovely_snake.h food.h compet.h common_ds.h
	g++ -c -O1 system.cxx

snake.o:snake.cxx const_dec.h lovely_snake.h food.h system.h compet.h
	g++ -c -O1 snake.cxx  
	
compet.o:compet.h const_dec.h compet.cxx common_ds.h
	g++ -c -O1 compet.cxx
	
ani.o:compet.h const_dec.h animation.cxx common_ds.h
	g++ -c -O1 animation.cxx -o ani.o

common_ds.o:const_dec.h common_ds.h common_ds.cxx
	g++ -c -O1 common_ds.cxx

sound.o:sound.h sound.cxx const_dec.h
	g++ -c -O1 sound.cxx
	
debug:
	g++ lovely_snake.cxx food.cxx system.cxx snake.cxx common_ds.cxx snake_list.cxx  compet.cxx animation.cxx sound.cxx -g -lncurses -o snake

install:
	sh mk_installer ${INSTALL_DIR}
	sh mk_installer ${AUDIO_DIR}
	-cp snake ${INSTALL_DIR}
	-cp -r audio/* ${AUDIO_DIR}
	-ln -s ${INSTALL_DIR}/snake ${SHADOW_DIR}/snake
	
uninstall:
	-yes|rm -r ${INSTALL_DIR}
	-rm ${SHADOW_DIR}/snake

clear:
	-rm -f snake.o data_imp.o compet.o ani.o common_ds.o
	-rm -f lovely_snake.o food.o system.o snake_list.o sound.o
	
clean:
	-rm -f snake.o data_imp.o compet.o ani.o common_ds.o	
	-rm -f lovely_snake.o food.o system.o snake_list.o sound.o
		
	-rm -f snake

