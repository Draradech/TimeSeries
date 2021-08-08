g++ -std=gnu++11 -D_GNU_SOURCE -Wall -o resample Resample.cpp DataTarget.cpp DataSource.cpp
g++ -std=gnu++11 -D_GNU_SOURCE -Wall -o aggregate Aggregate.cpp DataTarget.cpp DataSource.cpp
g++ -std=gnu++11 -D_GNU_SOURCE -Wall -o graph Graph.cpp DataSource.cpp Image.cpp
g++ -std=gnu++11 -D_GNU_SOURCE -Wall -o graphlog GraphLog.cpp DataSource.cpp Image.cpp
g++ -std=gnu++11 -D_GNU_SOURCE -Wall -o dump Dump.cpp DataSource.cpp
