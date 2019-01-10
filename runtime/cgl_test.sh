num=$1
cmdPath=/home/dongjitang/clustergl-master_multiserver_network/runtime
run_sh=${cmdPath}/run.sh
run_client_sh=${cmdPath}/run_client.sh
cd $cmdPath
echo `pwd`
echo "set up run.sh..."
exec $run_sh 1>/dev/null 2>&1 &
for i in `seq 1 $num`
do
    echo "before set up " ${i} "run_client.shd..."
    exec $run_client_sh 1>/dev/null 2>&1  &
    echo "after set up " ${i} "run_client.sh..."
done
