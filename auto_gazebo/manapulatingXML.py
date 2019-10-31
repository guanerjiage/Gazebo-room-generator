import xml.etree.ElementTree as ET

tree = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/try.world')
root = tree.getroot()

for models in root.find('world/model'):
    print(models.get('name'))
    if models.get('name') == 'wall':
        print(models.attrib)
        root.remove(models)

tree.write('output.world')

    

