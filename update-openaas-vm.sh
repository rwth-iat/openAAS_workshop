#!/bin/bash
#Start logging to file
exec > >(tee -ia /home/openaas/Documents/update.log)
exec 2> >(tee -ia /home/openaas/Documents/update.err)

date >&2
echo "________________________________________________________________________"
echo "Update openaas_workshop repository:"
cd /home/openaas/openaas_workshop/
sshpass -p openaas git pull --no-edit pi@$1:/home/pi/openaas_workshop
echo openaas | sudo -S git clean -fd
git reset --hard
cmake .
make

echo ""
echo "________________________________________________________________________"
echo "Installing opcua python interface"
cd python
echo openaas | sudo -S pip3 install -e .
cd ..
cp -f -v libreoffice/opcua_interface.py /home/openaas/.config/libreoffice/4/user/Scripts/python/opcua_interface.py

echo ""
echo "________________________________________________________________________"
echo "Copying openAAS libre office client"
cp -f -v libreoffice/aas_client.ods /home/openaas/Desktop/AAS\ Libre\ Office\ Client


echo ""
echo "Done."
echo ""
