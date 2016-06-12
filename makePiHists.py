#!/usr/bin/env python

from helpers import *
import ROOT as root
import glob

categories = [
  "collision",
  "elastic",
  "inelastic",
  "single_qx",
  "double_qx",
  "absorption",
  "normal_scatter",
]

colorMap = {
  "collision":root.kBlack,
  "elastic":root.kBlue,
  "inelastic":root.kCyan,
  "single_qx":root.kMagenta,
  "double_qx":root.kRed,
  "absorption":root.kGreen,
  "normal_scatter":root.kOrange,
}

c = root.TCanvas("c1")
c.SetLogy()
divideHist = None
allHists = []

for fn in glob.glob("pi*.hists.root"):
  isPip = True
  probeStr = "#pi^{+}"
  if fn[:3] == "pim":
    isPip = False
    probeStr = "#pi^{#minus}"
  isAr = True
  targetStr = "Ar"
  if fn[3] == "H":
    isAr = False
    targetStr = "H"
  title1Str = probeStr + " + " + targetStr
  c.Clear()
  f = root.TFile(fn)
  hists = []
  firstHist = True
  #####
  leg = root.TLegend(0.7,0.626,0.9,0.886)
  for cat in categories:
    hist = f.Get("p_"+cat)
    hist.UseCurrentStyle()
    hist.SetLineColor(colorMap[cat])
    hist.SetMarkerColor(colorMap[cat])
    hist.SetMarkerSize(0.0)
    #hist.Rebin()
    hists.append(hist)
    allHists.append(hist)
    if firstHist:
      hist.GetXaxis().SetRangeUser(0,5)
      if isPip:
        hist.GetXaxis().SetTitle(r"p_{#pi^{+}} [GeV]")
      else:
        hist.GetXaxis().SetTitle(r"p_{#pi^{#minus}} [GeV]")
      hist.GetYaxis().SetTitle(r"Simulated Events / bin")
      hist.GetYaxis().SetTitleOffset(1.5)
      hist.GetYaxis().SetRangeUser(0.1,1e6)
      hist.Draw("")
    else:
      hist.Draw("same")
    leg.AddEntry(hist,cat,"le")
    firstHist = False
  leg.Draw()
  c.RedrawAxis()
  
  tlatex = root.TLatex()
  tlatex.SetNDC()
  tlatex.DrawLatex(0.3,0.8,title1Str)

  outfn = "pi_p_"+fn[:-11]+".png"
  c.SaveAs(outfn)

  #########################################
  c.Clear()
  effs = []
  firstHist = True
  #####
  #c.SetLogy(False)
  #taxisHist = root.TH2F("taxis_"+fn,"",1,0.,5.,1,0,1.5)
  taxisHist = root.TH2F("taxis_"+fn,"",1,0.,5.,1,1e-3,1e2)
  if isPip:
    taxisHist.SetTitle(r";p_{#pi^{#plus}} [GeV];Fraction of Collisions")
  else:
    taxisHist.SetTitle(r";p_{#pi^{#minus}} [GeV];Fraction of Collisions")
  taxisHist.Draw()
  leg = root.TLegend(0.7,0.626,0.9,0.886)
  for cat in categories:
    hist = f.Get("p_efftocollision_"+cat)
    hist.SetTitle(";p;Fraction of Inelastic Collisions")
    hist.SetLineWidth(gStyle.GetHistLineWidth())
    hist.SetLineColor(colorMap[cat])
    hist.SetMarkerColor(colorMap[cat])
    hist.SetMarkerSize(0.0)
    #hist.Rebin()
    effs.append(hist)
    hist.Draw("Zsame")
    leg.AddEntry(hist,cat,"le")
    firstHist = False
  leg.Draw("")
  c.RedrawAxis()
  
  tlatex = root.TLatex()
  tlatex.SetNDC()
  tlatex.DrawLatex(0.3,0.8,title1Str)

  outfn = "pi_p_frac_"+fn[:-11]+".png"
  c.SaveAs(outfn)
  
