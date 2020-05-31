# r.accumulate.lessmem

`r.accumulate.lessmem` calculates weighted flow accumulation, stream networks, and the longest flow path using a flow direction map. It is a modified version of [`r.accumulate`](https://grass.osgeo.org/grass78/manuals/addons/r.accumulate.html) and requires less memory. It can be slower than [`r.accumulate`](https://grass.osgeo.org/grass78/manuals/addons/r.accumulate.html) because of heavy bit operations.
