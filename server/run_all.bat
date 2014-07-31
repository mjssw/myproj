cd Center
start Center.exe ../serverconfig/center.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd GroupManager
start GroupManager.exe ../serverconfig/GroupManager.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd GameManager
start GameManager.exe ../serverconfig/tetris_gamemgr.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd Group
start Group.exe ../serverconfig/Group.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd GroupGate
start GroupGate.exe ../serverconfig/GroupGate.xml ../netconfig.xml gate_cfg rpc_cfg
cd ..

cd Login
start Login.exe ../serverconfig/login.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

cd Tetris
start Tetris.exe ../serverconfig/tetris.xml ../netconfig.xml rpc_cfg rpc_cfg
cd ..

ping 127.0.0.1 -n 3 > null

cd Gate
start Gate.exe ../serverconfig/login_gate.xml ../netconfig.xml gate_cfg rpc_cfg
ping 127.0.0.1 -n 3 > null
start Gate.exe ../serverconfig/tetris_gate.xml ../netconfig.xml gate_cfg rpc_cfg
cd ..
