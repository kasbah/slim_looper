# This file is part of SLim Looper.
# 
# SLim Looper is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as 
# published by the Free Software Foundation.
# 
# SLim Looper is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with SLim Looper. If not, see <http://www.gnu.org/licenses/>

import lilv
import sys, os
import unittest
import numpy
class LooperTestCase(unittest.TestCase):
    def setUp(self):
        self.nframes = 64
        self.world = lilv.World()
        location = "file://" + os.getcwd() + "/build/src/slim_looper.lv2/"
        #location = "file://home/kaspar/.lv2/eg-amp.lv2/"
        self.plugin_uri = self.world.new_uri(location)
        self.assertIsNotNone(self.plugin_uri, "Invalid URI: '" + location + "'")

        self.world.load_bundle(self.plugin_uri)
        self.plugins = self.world.get_all_plugins()
        self.plugin =  self.plugins.get(self.plugins.begin())
        self.assertIsNotNone(self.plugin, msg="Plugin not found at location: '" + location + "'")
        self.assertEqual(location, self.plugin.get_bundle_uri().as_string())

        self.instance = lilv.Instance(self.plugin, 48000, None)

        self.in_buf           = numpy.array([1.0] * self.nframes, 'f')
        self.out_buf          = numpy.array([1.0] * self.nframes, 'f')
        self.record_buf       = numpy.array([0.0], 'f')
        self.pause_buf        = numpy.array([0.0], 'f')
        self.record_mode_buf  = numpy.array([0.0], 'f')

        self.instance.connect_port(0, self.in_buf)
        self.instance.connect_port(1, self.out_buf)
        self.instance.connect_port(2, self.record_buf)
        self.instance.connect_port(3, self.pause_buf)
        self.instance.connect_port(4, self.record_mode_buf)
        self.instance.activate()
    def setBufferAll(self, buf, val):
        for i,s in enumerate(buf):
            buf[i] = val
    def setInputAll(self, val):
        self.setBufferAll(self.in_buf, val)
    def setOutputAll(self, val):
        self.setBufferAll(self.out_buf, val)
    def assertBufferIsAll(self, buf, val, msg=None):
        for i,sample in enumerate(buf):
            if msg is None:
                msg = "Buffer not all expected value: %0.8f. Sample %i is: %0.8f" % (val, i, sample)
            self.assertAlmostEqual(sample, val, 8, msg)
    def assertOutputIsAll(self, val):
        self.assertBufferIsAll(self.out_buf, val)
    def tearDown(self):
        self.instance.deactivate()



class RecordTestCase(LooperTestCase):
    def testSilence(self):
        ''' Test default silence while and while not recording.'''
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.0)

        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.0)

    def testRecord(self):
        ''' Test record works.'''
        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.record_buf[0] = 0.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.1)

        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.1)
    def testRecord2(self):
        ''' Test record over two run calls.'''
        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)

        self.setInputAll(0.2)
        self.instance.run(self.nframes)

        self.record_buf[0] = 0.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.1)

        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.2)
    def testRecord3(self):
        ''' Test that subsequent records overwrite each other.'''
        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)

        self.record_buf[0] = 0.0
        self.instance.run(self.nframes)

        self.setInputAll(0.2)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)

        self.record_buf[0] = 0.0
        self.instance.run(self.nframes)

        self.assertOutputIsAll(0.2)
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.2)
    def testPause(self):
        '''Test that pause works.'''
        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.record_buf[0] = 0.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.1)
        self.pause_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.0)
        self.pause_buf[0] = 0.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.1)
    def testOverdub(self):
        '''Test that overdub works.'''
        self.setInputAll(0.1)
        self.record_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.record_mode_buf[0] = 1.0
        self.instance.run(self.nframes)
        self.assertOutputIsAll(0.2)

        
         
