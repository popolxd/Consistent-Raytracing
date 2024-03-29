# Consistent Raytracing

The idea is that it shouldn't include any noise, I am not following the reflection of a non-mirror (rough) surface, it was made so it can be suitable for some game. It should be more efficient the classical raytracing, at least when there are not a lot of lights.

Main benefit that is completely noise free and still looks fairly realistic.
It is not physically based!

Basic info:
    - no raybounces were used (shadow rays were utilized)

Lighting of object:
    - no refraction
    - phong reflection (diffuse, ambient and specular light)
    - shadow rays, mirror reflection (modified by me)

Lightsources:
    - directional light
    - point light

To fix and upgrade:
    - Perfect mirror mutual reflection breaks down
    - mirrors works fine, but is shouldeither be prefect mirror or perfect non-mirror, otherwise it produces pretty unrealistic results
    - fix the specular light problem (in shadow cannot be any specular light, but when looking at an angle, it doesn't look correct)
    - try to implement emmisive materials (not likely it is going to work)
    - try to make more types of light sources
    - try to make refraction possible (distant future)
    - general optimizations