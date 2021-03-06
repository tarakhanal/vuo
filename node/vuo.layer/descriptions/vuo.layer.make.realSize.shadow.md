Creates a layer that can be combined with other layers to create a composite image. 

The image is drawn with a shadow behind it.  You can control the shadow's color, focus, and placement.

Since this node creates a Real Size layer, the image used to create the layer will be rendered at its original size and rotation in the composite image. If you want to scale and rotate the layer, use the [Make Image Layer with Shadow (Scaled)](vuo-node://vuo.layer.make.shadow) node instead.

   - `Name` — A name to identify the layer, allowing other nodes to select it from a group of layers. 
   - `Image` — The image displayed in the layer. For the layer to be visible, you have to provide an image. 
   - `Center` — The center point of the image, in Vuo Coordinates.  The center point is snapped so that it aligns exactly with a pixel.
   - `Opacity` — The image's opacity, from 0 (fully transparent) to 1 (fully opaque). 
   - `Preserve Physical Size` — When true, the image is rendered at the same physical size on both Retina and non-Retina screens, and the image's Scale Factor specifies the image's natural size (see [vuo.layer](vuo-nodeset://vuo.layer)).  When false, each pixel in the image is rendered as 1 pixel on the screen.
   - `Shadow Color` — The color of the layer's shadow.  Use the color's opacity value to control the intensity of the shadow.
   - `Shadow Blur` — The amount to blur the layer's shadow, in pixels.  The blur extends this many pixels inward and this many pixels outward along the edge of the shadow. 
   - `Shadow Angle` — The shadow's angle relative to the layer, in degrees. At 0, the shadow is to the right of the layer. At 90, the shadow is above the layer. 
   - `Shadow Distance` — The distance that the shadow is offset from the layer, in Vuo Coordinates. 
