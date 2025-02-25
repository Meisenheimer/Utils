import imageio


with imageio.get_writer(uri='res.gif', mode='I', fps=10) as writer:
    t = 0.0
    while (t < 90.05):
        filename = "res/h_%.1f.png" % t
        print(filename)
        writer.append_data(imageio.imread(filename))
        t += 0.1
