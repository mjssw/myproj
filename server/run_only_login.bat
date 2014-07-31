cd Center
start Center.exe ../serverconfig/center.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd Login
start Login.exe ../serverconfig/login.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd Gate
start Gate.exe ../serverconfig/login_gate.xml ../netconfig.xml gate_cfg rpc_cfg
cd ..