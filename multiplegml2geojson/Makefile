build: main.o utils.o argumentParser.o featureExtractor.o utils.o
	g++ main.o argumentParser.o featureExtractor.o utils.o -o main -I/m/home/home5/52/kaarias3/unix/.conda/envs/geo/include -L/m/home/home5/52/kaarias3/unix/.conda/envs/geo/lib -lboost_system -lboost_filesystem -std=c++0x -pthread -lgdal -lboost_thread -lboost_program_options

main.o: main.cpp argumentParser.h featureExtractor.h utils.h
	g++ -c main.cpp -I/m/home/home5/52/kaarias3/unix/.conda/envs/geo/include -L/m/home/home5/52/kaarias3/unix/.conda/envs/geo/lib -lgdal -std=c++0x -pthread -lgdal -lboost_thread

argumentParser.o: argumentParser.h argumentParser.cpp 
	g++ -c -lboost_program_options argumentParser.cpp

featureExtractor.o: featureExtractor.h featureExtractor.cpp 
	g++ -c featureExtractor.cpp -I/m/home/home5/52/kaarias3/unix/.conda/envs/geo/include -L/m/home/home5/52/kaarias3/unix/.conda/envs/geo/lib -lgdal

utils.o: utils.h utils.cpp
	g++ -c -lboost_filesystem -I/m/home/home5/52/kaarias3/unix/.conda/envs/geo/include -L/m/home/home5/52/kaarias3/unix/.conda/envs/geo/lib -lgdal utils.cpp

clean:
	rm *.o || true
	rm main || true
	rm *geojson || true
