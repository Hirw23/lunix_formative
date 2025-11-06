# setup.py
from setuptools import setup, Extension

sim_ext = Extension(
    'simulator',
    sources=['simulator.c'],
    extra_compile_args=[],
)

setup(
    name='simulator',
    version='0.1',
    description='Lightweight IoT simulator extension',
    ext_modules=[sim_ext],
)

