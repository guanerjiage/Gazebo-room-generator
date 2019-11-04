import xml.etree.ElementTree as ET

tree = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/init.world')
root = tree.getroot()
world = root.find('world')


# erase the current wall
for something in root.find('world'):
    #print(something.tag)  
    if something.get('name') == 'wall':
        #print(something.attrib)
        world.remove(something)
'''
for something in world.find('state'):
    #print(something.tag)
    if something.get('name') == 'wall':
        #print(something.attrib)
        state.remove(something)
'''
# get the handler of tag model in newshape file
newshapefile = ET.parse('/home/hiwi03/Roomgenerator/auto_gazebo/shapes/shape0.sdf')
newshape_root = newshapefile.getroot()
newshape = newshape_root.find('model')

# changing size and location and adding shape into the world file
n_pose = newshape.find('pose')
# in the form of (x,y,z,alpha,beta,garmma)
n_pose.text = '3 4 0 0 -0 0'
# todo how to change size of the model
world.append(newshape)


tree.write('output.world')