#KR Server, a program for creating servers and customizing them with Network Scripts.
#Copyright (C) 2023 kevidryon2
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Affero General Public License as
#published by the Free Software Foundation, either version 3 of the
#License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Affero General Public License for more details.
#
#You should have received a copy of the GNU Affero General Public License
#along with this program.  If not, see <https://www.gnu.org/licenses/>.
all: clean build/krsrv build/netcc

clean:
	rm -f build/*

build/krsrv: build/httpserver.o build/main.o build/bnsload.o build/libksl.o
	gcc -g build/httpserver.o build/main.o build/bnsload.o build/libksl.o -o build/krsrv
	
build/krsrv_portable: build/httpserver.o build/main.o build/bnsload.o build/libksl.o
	gcc -g build/httpserver.o build/main.o build/bnsload.o build/libksl.o -static -o build/krsrv_portable

build/netcc: build/netc.o build/libnetc.o build/libksl.o
	gcc -g build/*netc.o build/libksl.o -dynamic -o build/netc

build/%.o: src/%.c
	gcc -g -c -o $@ $<

