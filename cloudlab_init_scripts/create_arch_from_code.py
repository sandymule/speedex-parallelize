"""
basic topology with nodes in a line and switches between them

"""

import geni.portal as portal

import geni.rspec.pg as rspec

portal.context.defineParameter("n", "Number of instances", portal.ParameterType.INTEGER, 2)

portal.context.defineParameter("hardware", "Hardware Type", portal.ParameterType.NODETYPE, "c6525-25g")

params = portal.context.bindParameters()

request = portal.context.makeRequestRSpec()

portal.context.verifyParameters()

nodes = []
for i in range(0, params.n):
    node = request.RawPC("node-" + str(i))
    node.hardware_type = params.hardware
    node.disk_image = "urn:publicid:IDN+utah.cloudlab.us+image+edce-PG0:single-machine"
    node.addService(rspec.Execute(shell="bash", command = "sudo bash /opt/init_from_script.sh"))
    nodes.append(node)

link = request.Link(members = nodes)


portal.context.printRequestRSpec()
