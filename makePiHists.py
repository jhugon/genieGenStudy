#!/usr/bin/env python

from helpers import *
import ROOT as root
root.gROOT.SetBatch(True)
import glob

allFiles = []
allHists = []

def makeXSHists(fn,histPrefix,categories):
  """
  Makes a list of cross-section histograms from
    an input file, histPrefix ("ke_" or "p_")
    and a list of category names.
  """
  isAr = True
  targetA = 40
  outfnRun = fn.split(".")[0]
  if outfnRun[-1] == "H":
    isAr = False
    targetA = 1
  f = root.TFile(fn)
  allFiles.append(f)
  hists = []
  allHist = f.Get(histPrefix+"all")
  allHists.append(allHist)
  for cat in categories:
    hist = f.Get(histPrefix+cat)
    ####
    hist.Divide(allHist)
    fm2tomb = 10.
    NR      = 3.0
    R0      = 1.4
    R = NR * R0 * targetA**0.3333
    S = fm2tomb * math.pi * R**2
    hist.Scale(S)
    ####
    hists.append(hist)
    allHists.append(hist)
  return hists

def plotHists(hists,titles,colors,filenameprefix,xtitle="",ytitle="#sigma [mb]",suptitle=""):
  c.Clear()
  axisHist = makeStdAxisHist(hists,logy=c.GetLogy())
  setHistTitles(axisHist,xtitle,ytitle)
  axisHist.Draw()
  firstHist = True
  #####
  for hist,title,color in zip(hists,titles,colors):
    hist.UseCurrentStyle()
    hist.SetLineWidth(2)
    hist.SetLineColor(color)
    hist.SetMarkerColor(color)
    hist.SetMarkerSize(0.0)
    allHists.append(hist)
    hist.Draw("histsame")
  leg = drawNormalLegend(hists,titles,option="l")
  drawStandardCaptions(c,suptitle,preliminaryString="GENIE Simulation")
  c.RedrawAxis()
  
  outfn = filenameprefix+".png"
  c.SaveAs(outfn)
  outfn = filenameprefix+".pdf"
  c.SaveAs(outfn)

def plotStack(hists,titles,colors,filenameprefix,xtitle="",ytitle="#sigma [mb]",suptitle=""):
  c.Clear()
  stack = root.THStack()
  sumHist = None
  for hist,color in zip(reversed(hists),reversed(colors)):
    hist.UseCurrentStyle()
    hist.SetLineColor(color)
    hist.SetMarkerColor(color)
    hist.SetFillColor(color)
    hist.SetMarkerSize(0.0)
    allHists.append(hist)
    stack.Add(hist)
    if sumHist is None:
        sumHist = hist.Clone(hist.GetName()+"_sumHist")
    else:
        sumHist.Add(hist)
  axisHist = makeStdAxisHist([sumHist],logy=c.GetLogy())
  setHistTitles(axisHist,xtitle,ytitle)
  axisHist.Draw()
  stack.Draw("histsame")
  #####
  leg = drawNormalLegend(hists,titles,option="f")
  drawStandardCaptions(c,suptitle,preliminaryString="GENIE Simulation")
  c.RedrawAxis()
  
  outfn = filenameprefix+".png"
  c.SaveAs(outfn)
  outfn = filenameprefix+".pdf"
  c.SaveAs(outfn)

c = root.TCanvas("c1")
allCollision = []
allElastic = []
allInelastic = []
allTitles = []
allPiTitles = []
for fn in sorted(glob.glob("*.hists.root")):
  probeStr = "??"
  if fn[:3] == "pip":
    probeStr = "#pi^{+}"
  elif fn[:3] == "pim":
    probeStr = "#pi^{#minus}"
  elif fn[:2] == "kp":
    probeStr = "K^{+}"
  elif fn[:2] == "km":
    probeStr = "K^{#minus}"
  elif fn[:1] == "p":
    probeStr = "p"
  outfnRun = fn.split(".")[0]
  targetStr = "Ar"
  if outfnRun[-1] == "H":
    isAr = False
    targetStr = "H"
  suptitle = probeStr + " + " + targetStr
  allTitles.append(suptitle)

  print suptitle,fn

  categories = [
    "collision",
    "scatter_1deg",
    "scatter_2deg",
    "scatter_5deg",
    "scatter_10deg",
  ]

  titles = [
    "Collision",
    "Scatter >1#circ",
    "Scatter >2#circ",
    "Scatter >5#circ",
    "Scatter >10#circ",
  ]

  hists = makeXSHists(fn,"ke_",categories)
  allCollision.append(hists[0])
  c.SetLogy(False)
  plotHists(hists,titles,[1]+COLORLIST[:len(hists)-1],"ke_angle_"+outfnRun,xtitle="KE_{"+probeStr+"} [MeV]",suptitle=suptitle)

  hists = makeXSHists(fn,"ke_",["elastic","inelastic"])
  c.SetLogy(False)
  plotStack(hists,["Elastic","Inelastic"],COLORLIST[:len(hists)],"ke_lastic_"+outfnRun,xtitle="KE_{"+probeStr+"} [MeV]",suptitle=suptitle)

  allElastic.append(hists[0])
  allInelastic.append(hists[1])

  if fn[:2] != "pi":
    continue

  categories = [
    "elastic",
    "absorption",
    "other",
    "single_qx",
    "double_qx",
  ]

  titles = [
    "Elastic",
    "Absorption",
    "Other",
    "Single Ch. Ex.",
    "Double Ch. Ex.",
  ]


  hists = makeXSHists(fn,"ke_",categories)
  c.SetLogy(False)
  plotHists(hists,titles,COLORLIST[:len(hists)],"ke_type_"+outfnRun,xtitle="KE_{"+probeStr+"} [MeV]",suptitle=suptitle)
  plotStack(hists,titles,COLORLIST[:len(hists)],"ke_type_stack_"+outfnRun,xtitle="KE_{"+probeStr+"} [MeV]",suptitle=suptitle)


  allPiTitles.append(suptitle)

allArTitles = [i for i in allTitles if "Ar" in i]
allArCollision = [allCollision[i] for i in range(len(allTitles)) if "Ar" in allTitles[i]]
allArElastic = [allElastic[i] for i in range(len(allTitles)) if "Ar" in allTitles[i]]
allArInelastic = [allInelastic[i] for i in range(len(allTitles)) if "Ar" in allTitles[i]]

plotHists(allArCollision,allArTitles,COLORLIST[:len(allArCollision)],"all_ke_collision",xtitle="KE [MeV]",suptitle="Collision")
plotHists(allArElastic,allArTitles,COLORLIST[:len(allArElastic)],"all_ke_elastic",xtitle="KE [MeV]",suptitle="Elastic")
plotHists(allArInelastic,allArTitles,COLORLIST[:len(allArInelastic)],"all_ke_inelastic",xtitle="KE [MeV]",suptitle="Inelastic")
