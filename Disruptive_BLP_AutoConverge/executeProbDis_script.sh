# !/bin/bash

#  executeProbDisruptiveBLP.sh
#  Boost
#
#  Created by Steve DengZishi on 3/21/18.
#  Copyright © 2018 Steve DengZishi. All rights reserved.
echo -e "\nRandomly Initialized Balanced Label Propagation      Version 1.3"
echo -e "       Copyright © 2017 Steve DengZishi  New York University\n"

#set the source file for input
read -p "Enter the file name of the undirected graph: " FileName
read -p "Enter the number of partitions k (k<1000): " shard


#compile all .cpp files to executables
g++ -o clean clean.cpp -std=c++11
echo -e "\ng++ compiled clean.cpp successfully"
g++ -o lp_ingredient_producer lp_ingredient_producer.cpp -std=c++11
echo "g++ compiled lp_ingredient_producer.cpp successfully"
g++ -o linear linear.cpp -std=c++11
echo "g++ compiled linear.cpp successfully"
g++ -o applyMove applyMove.cpp -std=c++11
echo "g++ compiled applyMove.cpp successfully"
g++ -o probDisruptiveMove probDisruptiveMove.cpp -std=c++11
echo "g++ compiled disruptiveMove.cpp successfully"
g++ -o RandomAssignment RandomAssignment.cpp -std=c++11
echo -e "g++ compiled RandomAssignment.cpp successfully\n"
chmod +x checkConvergence.py

#time the effective execution
time(
#initialize using random assignment outside of iteration
echo -e "Starting random initialization...\n"
./RandomAssignment $FileName $shard


echo -e "Initialization completed\n"

#to make sure do not run two disruptive rounds in a row
skip=0
last=0
i=0
#start of iteration
while true

do
((++i))
echo "In iteration" $i

#after the first two rounds, start to check whether result converges
if (($i>2)) && (($skip==0))

then
#declare array to store the returned values
result=($(./checkConvergence.py))

#echo ${result[0]}
#echo ${result[1]}

if [ "${result[0]}" == "TRUE" ]

then
echo "Increase in locality converges"

if (( $(echo "${result[1]} > $last" | bc -l) ))

then
echo "Disruptive condition met, running disruptive round"

./probDisruptiveMove $FileName $shard

skip=1

last=${result[1]}

else
echo -e "Converges, ending Balanced Label Propagation\n"
echo "The highest locality is: $last"
break

fi

else
#taking too much time for large graph
./lp_ingredient_producer $FileName $shard > lp_ingred.txt

./linear < lp_ingred.txt | lp_solve | ./clean | sort > x_result_$i.txt

x_file=x_result_$i.txt
#too much time for large graph, map to move large time for many partitions
./applyMove $FileName $shard $x_file

skip=0
fi

else
./lp_ingredient_producer $FileName $shard > lp_ingred.txt

./linear < lp_ingred.txt | lp_solve | ./clean | sort > x_result_$i.txt

x_file=x_result_$i.txt

./applyMove $FileName $shard $x_file

skip=0
fi

done

)

#plotting graph after finish looping
chmod +x graph_plot.py
./graph_plot.py
