import imageio.v2 as imageio


with imageio.get_writer(uri='res.gif', mode='I', fps=10) as writer:
    for i in range(256, 65536 + 1, 256):
        filename = "res/%d.png" % i
        print(filename)
        writer.append_data(imageio.imread(filename))
