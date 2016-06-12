#!/usr/bin/env python

from helpers import *
import ROOT as root
import glob

categories = [
  #"all",
  "cc qel",
  "cc res",
  "cc dis",
  "nc qel",
  "nc res",
  "nc dis",
  #"cc",
  #"nc",
]

colorMap = {
  "all":root.kBlack,
  "cc qel":root.kBlue,
  "cc res":root.kGreen,
  "cc dis":root.kRed,
  "nc qel":root.kCyan,
  "nc res":root.kYellow,
  "nc dis":root.kMagenta,
  "cc":root.kBlack,
  "nc":root.kGray,
}

c = root.TCanvas("c1")
axisHist = root.TH2F("axisHist","",1,0,20,1,0,1.0)
setHistTitles(axisHist,r"E_{#nu} [GeV]",r"#sigma / E_{#nu} / nucleon [10^{-38} cm^{2} / GeV]")

for fn in glob.glob("*.gst.root"):
  graphMap = {}
  leg = root.TLegend(0.6,0.626,0.9,0.886)
  for cat in categories:
    graph = root.TGraph()
    graph.SetMarkerColor(colorMap[cat])
    graphMap[cat] = graph
    leg.AddEntry(graph,cat,"p")
  weightfn = fn[:-9]+".rewght.root"
  c.Clear()
  f = root.TFile(fn)
  fweights = root.TFile(weightfn)
  tree = f.Get("gst")
  weights = fweights.Get("weights")
  for i in range(tree.GetEntries()):
    tree.GetEntry(i)
    if tree.A < 1:
      continue
    weights.GetEntry(i)
    tree.Ev, weights.xsec
    if tree.qel and tree.cc:
      graphMap['cc qel'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    if tree.res and tree.cc:
      graphMap['cc res'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    if tree.dis and tree.cc:
      graphMap['cc dis'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    if tree.qel and tree.nc:
      graphMap['nc qel'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    if tree.res and tree.nc:
      graphMap['nc res'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    if tree.dis and tree.nc:
      graphMap['nc dis'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    #if tree.cc:
    #  graphMap['cc'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
    #if tree.nc:
    #  graphMap['nc'].SetPoint(i,tree.Ev,weights.xsec/tree.Ev/tree.A)
  axisHist.Draw()
  graphMap["cc qel"].Draw("P")
  graphMap["cc res"].Draw("P")
  graphMap["cc dis"].Draw("P")
  leg.Draw()
  c.SaveAs("graph_CC_"+fn[:-9]+".png")
  axisHist.Draw()
  graphMap["nc qel"].Draw("P")
  graphMap["nc res"].Draw("P")
  graphMap["nc dis"].Draw("P")
  leg.Draw()
  c.SaveAs("graph_NC_"+fn[:-9]+".png")
  axisHist.Draw()
  graphMap["nc qel"].Draw("P")
  graphMap["cc qel"].Draw("P")
  leg.Draw()
  c.SaveAs("graph_QEL_"+fn[:-9]+".png")
  
    
  
  

