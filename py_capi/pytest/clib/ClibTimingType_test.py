#! /usr/bin/env python3

""" ClibTimingTypeのテストスクリプト

:file: py_ClibTimingType_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.clib import ClibTimingType


def test_clibtimingtype_combinational():
    val1 = ClibTimingType("combinational")
    assert val1 == ClibTimingType.combinational
    assert val1.__repr__() == "combinational"

def test_clibtimingtype_combinational_rise():
    val1 = ClibTimingType("combinational_rise")
    assert val1 == ClibTimingType.combinational_rise
    assert val1.__repr__() == "combinational_rise"

def test_clibtimingtype_combinational_fall():
    val1 = ClibTimingType("combinational_fall")
    assert val1 == ClibTimingType.combinational_fall
    assert val1.__repr__() == "combinational_fall"

def test_clibtimingtype_three_state_enable():
    val1 = ClibTimingType("three_state_enable")
    assert val1 == ClibTimingType.three_state_enable
    assert val1.__repr__() == "three_state_enable"

def test_clibtimingtype_three_state_disable():
    val1 = ClibTimingType("three_state_disable")
    assert val1 == ClibTimingType.three_state_disable
    assert val1.__repr__() == "three_state_disable"

def test_clibtimingtype_three_state_enable_rise():
    val1 = ClibTimingType("three_state_enable_rise")
    assert val1 == ClibTimingType.three_state_enable_rise
    assert val1.__repr__() == "three_state_enable_rise"

def test_clibtimingtype_three_state_enable_fall():
    val1 = ClibTimingType("three_state_enable_fall")
    assert val1 == ClibTimingType.three_state_enable_fall
    assert val1.__repr__() == "three_state_enable_fall"

def test_clibtimingtype_three_state_disable_rise():
    val1 = ClibTimingType("three_state_disable_rise")
    assert val1 == ClibTimingType.three_state_disable_rise
    assert val1.__repr__() == "three_state_disable_rise"

def test_clibtimingtype_three_state_disable_fall():
    val1 = ClibTimingType("three_state_disable_fall")
    assert val1 == ClibTimingType.three_state_disable_fall
    assert val1.__repr__() == "three_state_disable_fall"

def test_clibtimingtype_rising_edge():
    val1 = ClibTimingType("rising_edge")
    assert val1 == ClibTimingType.rising_edge
    assert val1.__repr__() == "rising_edge"

def test_clibtimingtype_falling_edge():
    val1 = ClibTimingType("falling_edge")
    assert val1 == ClibTimingType.falling_edge
    assert val1.__repr__() == "falling_edge"

def test_clibtimingtype_preset():
    val1 = ClibTimingType("preset")
    assert val1 == ClibTimingType.preset
    assert val1.__repr__() == "preset"

def test_clibtimingtype_clear():
    val1 = ClibTimingType("clear")
    assert val1 == ClibTimingType.clear
    assert val1.__repr__() == "clear"

def test_clibtimingtype_hold_rising():
    val1 = ClibTimingType("hold_rising")
    assert val1 == ClibTimingType.hold_rising
    assert val1.__repr__() == "hold_rising"

def test_clibtimingtype_hold_falling():
    val1 = ClibTimingType("hold_falling")
    assert val1 == ClibTimingType.hold_falling
    assert val1.__repr__() == "hold_falling"

def test_clibtimingtype_setup_rising():
    val1 = ClibTimingType("setup_rising")
    assert val1 == ClibTimingType.setup_rising
    assert val1.__repr__() == "setup_rising"

def test_clibtimingtype_setup_falling():
    val1 = ClibTimingType("setup_falling")
    assert val1 == ClibTimingType.setup_falling
    assert val1.__repr__() == "setup_falling"

def test_clibtimingtype_recovery_rising():
    val1 = ClibTimingType("recovery_rising")
    assert val1 == ClibTimingType.recovery_rising
    assert val1.__repr__() == "recovery_rising"

def test_clibtimingtype_recovery_falling():
    val1 = ClibTimingType("recovery_falling")
    assert val1 == ClibTimingType.recovery_falling
    assert val1.__repr__() == "recovery_falling"

def test_clibtimingtype_skew_rising():
    val1 = ClibTimingType("skew_rising")
    assert val1 == ClibTimingType.skew_rising
    assert val1.__repr__() == "skew_rising"

def test_clibtimingtype_skew_falling():
    val1 = ClibTimingType("skew_falling")
    assert val1 == ClibTimingType.skew_falling
    assert val1.__repr__() == "skew_falling"

def test_clibtimingtype_removal_rising():
    val1 = ClibTimingType("removal_rising")
    assert val1 == ClibTimingType.removal_rising
    assert val1.__repr__() == "removal_rising"

def test_clibtimingtype_removal_falling():
    val1 = ClibTimingType("removal_falling")
    assert val1 == ClibTimingType.removal_falling
    assert val1.__repr__() == "removal_falling"

def test_clibtimingtype_non_seq_setup_rising():
    val1 = ClibTimingType("non_seq_setup_rising")
    assert val1 == ClibTimingType.non_seq_setup_rising
    assert val1.__repr__() == "non_seq_setup_rising"

def test_clibtimingtype_non_seq_setup_falling():
    val1 = ClibTimingType("non_seq_setup_falling")
    assert val1 == ClibTimingType.non_seq_setup_falling
    assert val1.__repr__() == "non_seq_setup_falling"

def test_clibtimingtype_non_seq_hold_rising():
    val1 = ClibTimingType("non_seq_hold_rising")
    assert val1 == ClibTimingType.non_seq_hold_rising
    assert val1.__repr__() == "non_seq_hold_rising"

def test_clibtimingtype_non_seq_hold_falling():
    val1 = ClibTimingType("non_seq_hold_falling")
    assert val1 == ClibTimingType.non_seq_hold_falling
    assert val1.__repr__() == "non_seq_hold_falling"

def test_clibtimingtype_nochange_high_high():
    val1 = ClibTimingType("nochange_high_high")
    assert val1 == ClibTimingType.nochange_high_high
    assert val1.__repr__() == "nochange_high_high"

def test_clibtimingtype_nochange_high_low():
    val1 = ClibTimingType("nochange_high_low")
    assert val1 == ClibTimingType.nochange_high_low
    assert val1.__repr__() == "nochange_high_low"

def test_clibtimingtype_nochange_low_high():
    val1 = ClibTimingType("nochange_low_high")
    assert val1 == ClibTimingType.nochange_low_high
    assert val1.__repr__() == "nochange_low_high"

def test_clibtimingtype_nochange_low_low():
    val1 = ClibTimingType("nochange_low_low")
    assert val1 == ClibTimingType.nochange_low_low
    assert val1.__repr__() == "nochange_low_low"

def test_clibtimingtype_bad():
    with pytest.raises(ValueError):
        ClibTimingType("abc")
